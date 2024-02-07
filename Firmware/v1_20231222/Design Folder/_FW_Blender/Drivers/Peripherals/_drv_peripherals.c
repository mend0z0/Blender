/****************************************************************************************************
*
*   Project Name:       Blender
*   @Brief Description: Blending audio spec with light spectrum
*   File Status:	    DRAFT   (DRAFT , PRELIMINARY, CHECKED, RELEASED)
*
*	File Name:	_drv_pheripherals.c
*	Version:	01
*	Revision:	01
*	Date:		2023/01/25
*	License:	Open-source
*	******************************** Project Description *******************************************
*	@Detailed Description: The Blender project's purpose is to extract digitalized sound characteristics
*   like amplitude and frequency, which are transferring through the S/PDIF protocol, and display the amplitude
*   of different frequencies by the dominant color of the input VGA received signal. So the base color will be
*   determined by VGA input, and a spectrum of that color will be displayed over/on the DMX/WS2812 chip. Also,
*   the specification of the sound will be displayed on an OLED, and the sound will be played by a chip that has
*   an integrated DAC in it. The data to DAC will be transferred via the I2S protocol.
*
*	******************************** File Description *******************************************
*	@Detailed Description:
*
*
*	*********************************  Hardware Info  **********************************************
*   Name:       _HW_Blender
*   Version:    v1.0
*	*********************************  Processor Info **********************************************
*	Processor:          STM32F446RCT6 32-Bits
*	Clock Frequency:    180.000	MHz
*	RAM/SRAM Size:      128.00	KBytes
*	Flash Size:         256.00	KBytes
*	***********************************  Case Style  ***********************************************
*	Local Variables = camelCase			Global Variables = g_camelCase
*	Headers = SCREAMIN_SNAKE_CASE
*   Macros = SCREAMING_SNAKE_CASE
*   Const Variables = SCREAMING_SNAKE_CASE
*	Functions = PascalCase				Initialize Functions = _lower_case
*	Struct = lower_case					Struct.members = camelCase
*	Enum = lower_case					Enum Members = SNAKE_CASE
*	Pointer Variables = p_camelCase
*	*********************************** Contact Info ***********************************************
*	Author:	Siavash Taher Parvar
*	Github: github.com/Mend0z0
****************************************************************************************************/

/****************************************************************************************************
****************************       HEADERS DECLARATION       ****************************************
*****************************************************************************************************/
#include "_drv_peripherals.h"

/****************************************************************************************************
****************************   GLOB. VARIABLES DECLARATION    ***************************************
*****************************************************************************************************/
SemaphoreHandle_t FMPI2CBinarySemaphore;
SemaphoreHandle_t USART1BinarySemaphore;

/****************************************************************************************************
****************************   CONST VARIABLES DECLARATION    ***************************************
*****************************************************************************************************/


/****************************************************************************************************
***********************     STATIC/LOCAL FUNCTIONS DECLARATION      *********************************
*****************************************************************************************************/
/*			Power controller 									*/
static void _init_PWR( void );

/*			Reset and clock control 							*/
static void _init_RCC( void );

/*			General-purpose I/Os								*/
static void _init_GPIO( void );

/*			Direct memory access controller						*/
static void _init_DMA( void );
static void DMAEnable( uint8_t channelNumber, uint8_t *data, uint32_t numOfTransfer);
void DMADisable( uint8_t channelNumber );

/*			Analog-to-digital converter	1						*/
static void _init_ADC1( void );

/*			Analog-to-digital converter	2						*/
static void _init_ADC2( void );

/*			Analog-to-digital converter	3						*/
static void _init_ADC3( void );

/*			Digital-to-analog converter							*/
static void _init_DAC( void );

/*			Advanced-control timers (TIM1)						*/
static void _init_TIM1( void );

/*			General-purpose timers (TIMER2)						*/
static void _init_TIM2( void );

/*			General-purpose timers (TIMER3)						*/
static void _init_TIM3( void );

/*			Real-time clock										*/
static void _init_RTC( void );

/*			Inter-integrated circuit (I2C) interface			*/
static void _init_FMPI2C1( void );

/*			Universal asynchronous receiver transmitter			*/
static void _init_UART1( void );

/*			Inter-IC sound										*/
static void _init_I2S( void );

/*			SPDIF receiver interface							*/
static void _init_SPDIF( void );

/****************************************************************************************************
****************************         GLOBAL FUNTIONS         ****************************************
*****************************************************************************************************/
void _init_Peripherals( void ){
	_init_PWR();
	_init_RCC();
	_init_GPIO();
	_init_DMA();
	_init_ADC1();
	_init_ADC2();
	_init_ADC3();
	_init_DAC();
	_init_TIM1();
	_init_TIM2();
	_init_TIM3();
	_init_RTC();
	_init_FMPI2C1();
	_init_UART1();
	_init_I2S();
	_init_SPDIF();
}

/*			General-purpose I/Os								*/

/*			Analog-to-digital converter							*/

/*			Advanced-control timers (TIM1)						*/

/*			General-purpose timers (TIM3)						*/
void TIM3Enable( void )
{
	TIM3->CNT = 0;
	TIM3->CR1 |= TIM_CR1_CEN;
}

void TIM3Disable( void )
{
	TIM3->CR1 &= ~TIM_CR1_CEN;
}

void TIM3UpdateCCR3( uint32_t *ccrValue )
{
	TIM3->CCR3 = *ccrValue;
}

/*			Real-time clock										*/

/*			Inter-integrated circuit (I2C) interface			*/
int32_t FMPI2C1DataTx( uint8_t slaveAddr, uint8_t *data, uint32_t buffSize)
{
	__IO int32_t status = 0;

	TickType_t semaphoreWatiTime = pdMS_TO_TICKS(10);	// 10msec delay for semaphore to get created, if it didn't happen immediately!
	FMPI2CBinarySemaphore = xSemaphoreCreateBinary();	// Creating semaphore for FMPI2C

	//If NULL is returned, then the semaphore cannot be created
	//because there is insufficient heap memory available for FreeRTOS
	//to allocate the semaphore data structures.
	if(FMPI2CBinarySemaphore == NULL){
		status = pdFALSE;
	}
	else{
		DMAEnable( DMA1_FMPI2C1_TX_EN, *data, buffSize);	// Init DMA with data address and number of transfer and then enable it.
		FMPI2C1->CR1 |= FMPI2C_CR1_TXIE 	| 					// Enable Transmit interrupt (it's mainly for confirming the sent addr)
						FMPI2C_CR1_NACKIE					// Enable NACK interrupt.
						;
		FMPI2C1->CR2 |= ((slaveAddr & 0X7F) << 1);			// Set Slave Address
		FMPI2C1->CR2 &= ~FMPI2C_CR2_RD_WRN;					// Set transfer direction to 0 to write on slave.
		FMPI2C1->CR2 |=	FMPI2C_CR2_START;					// Sending an start on line

		// At this point, we'll wait until we give the semaphore back from ISR
		xSemaphoreTake( FMPI2CBinarySemaphore, semaphoreWatiTime );
		//Once we've received the semaphore from ISR we'll check if there was an error or completion of transmitted data.
		DMADisable( DMA1_FMPI2C1_TX_DIS );
		if((FMPI2C1->ISR & FMPI2C_ISR_NACKF) == FMPI2C_ISR_NACKF)
		{
			FMPI2C1->ICR |= FMPI2C_ICR_NACKCF;		 // Clearing the NACK flag.
			status = FMPI2C_ERROR_NACK;				 // Returning error
		}
		else if((DMA1->HISR & DMA_HISR_TCIF5) == DMA_HISR_TCIF5)
		{
			DMA1->HIFCR |= DMA_HIFCR_CTCIF5;	//Clearing the flag by writing a one.
			status = (int32_t)buffSize;
		}
		else{
			status = FMPI2C_ERROR_UNKNOWN;
		}
	}

	return status;
}

/*			Universal asynchronous receiver transmitter			*/
int32_t UART1DataTx( uint8_t *data, uint32_t buffSize)
{
	__IO int32_t status = 0;

	TickType_t semaphoreWatiTime = pdMS_TO_TICKS(10);	// 10msec delay for semaphore to get created, if it didn't happen immediately!
	USART1BinarySemaphore = xSemaphoreCreateBinary();	// Creating semaphore for FMPI2C

	//If NULL is returned, then the semaphore cannot be created
	//because there is insufficient heap memory available for FreeRTOS
	//to allocate the semaphore data structures.
	if(USART1BinarySemaphore == NULL){
		status = pdFALSE;
	}
	else{
		DMAEnable( DMA2_UART1_TX_EN, *data, buffSize);	// Init DMA with data address and number of transfer and then enable it.
		USART1->CR1 |= USART_CR1_TCIE;					// Enabling the transmission complete interrupt
		USART1->CR1 |= USART_CR1_UE;					// Enabling the UART peripheral.
		// Creating a semaphore to hold the code here and send the data from DMA to USART
		xSemaphoreTake( USART1BinarySemaphore, semaphoreWatiTime);
		// After the Semaphore has been given we should disable DMA
		DMADisable( DMA2_UART1_TX_DIS );
		// Checking if the TC has been raised, in that case the transfer has been completed.
		if((USART1->SR & USART_SR_TC) == USART_SR_TC){
			USART1->SR &= ~USART_SR_TC;					// Clearing the TC flag.
			status = (int32_t)buffSize;
		}
		else
		{
			status = pdFALSE;
		}
	}
	USART1->CR1 &= ~USART_CR1_UE;					//Disable the UART peripheral.
	return status;
}

/*			Inter-IC sound										*/
int8_t I2SDataTx( uint32_t *data)
{
	//Init DMA with data address and number of transfer
	//Enable DMA
	//DMAEnable( DMA2_SPI1_TX_EN, *data, buffSize);
	return 0;
}

/*			SPDIF receiver interface							*/

/****************************************************************************************************
****************************         STATIC FUNTIONS         ****************************************
*****************************************************************************************************/
/*			Power controller 									*/
static void _init_PWR( void )
{
	//PROBABLY i NEED TO ENABLE THE CLOCK...????!?!?!
	//RCC->APB1ENR |= RCC_APB1ENR_PWREN;
	PWR->CR |= PWR_CR_VOS;						//selecting Scale mode 1
	PWR->CR |= PWR_CR_ODEN;						//Enabling the over drive
	while((PWR->CSR & PWR_CSR_ODRDY) == 0)		//waiting for the Over-drive mode ready flag to set
	{
		//maybe adding a time-out later
	}
	PWR->CR |= PWR_CR_ODSWEN;					//Enabling over drive switch to connect the 1.2 to domain.
	while((PWR->CSR & PWR_CSR_ODSWRDY) == 0)	//waiting for the Over-drive mode switching ready flag to set
	{
		//maybe adding a time-out later
	}
}

/*			Reset and clock control 							*/
static void _init_RCC( void )
{
	////--------------Enabling the HSE
	RCC->CR |= RCC_CR_HSEON;					//Enabling the external clock
	while((RCC->CR & RCC_CR_HSERDY) == 0)		//Waiting for the HSE clock ready flag to set
	{
		//maybe add a time out later
	}

	////--------------Configuration for PLLs
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC;			//Set the PLL clock source as HSE;

	/*
			HSE FREQ. = 16MHz
		Caution: The software has to set these bits correctly to ensure that the VCO output
		 			frequency is between 100 and 432 MHz.
			f(VCO clock) = f(PLL clock input) × (PLLN / PLLM)
			f(VCO clock) = HSE (16MHz) * (PLLN (180) / PLLM (8)) = 16 * 22.5 ~= 360 MHz
		 	f(PLL general clock output) = f(VCO clock) / PLLP
		 	f(PLL general clock output) = 360 (352) / 2 ~= 180MHz (176MHz)

	*/
	RCC->PLLCFGR |= ((	RCC_PLLCFGR_PLLN_2 	|
						RCC_PLLCFGR_PLLN_4 	|
						RCC_PLLCFGR_PLLN_5 	|
						RCC_PLLCFGR_PLLN_7) |
						RCC_PLLCFGR_PLLM_3);

	RCC->PLLCFGR |= RCC_PLLCFGR_PLLR_1;			// PPLR Division factor to 2 so the SPDIF and I2S clock is 90MHz.

	RCC->CR |= RCC_CR_PLLON;					//Enable the main PLL
	while((RCC->CR & RCC_CR_PLLRDY) == 0)		//Waiting for the main PLL locks
	{
		//maybe add a time out later
	}

	//Set the clock system source
	RCC->CFGR |= RCC_CFGR_SW_1;								//Set the clock source as PLLCLK
	while((RCC->CFGR & RCC_CFGR_SWS_1) != RCC_CFGR_SWS_1)	//Making sure the clock source is PLLCLK
	{
		//maybe add a time out later
	}

	//Configure AHB prescaler
	RCC->CFGR |= RCC_CFGR_HPRE_3;						//divide by 2 ~= 90MHz
	//Configure APB1 prescaler
	RCC->CFGR |= RCC_CFGR_PPRE1_0 | RCC_CFGR_PPRE1_2; 	//divide by 4 ~= 45MHz
	//Configure APB2 prescaler
	RCC->CFGR |= RCC_CFGR_PPRE2_2;						//divide by 2 ~= 90MHz

	//Enable GPIO (A, B, C, D)
	RCC->AHB1ENR |= (	RCC_AHB1ENR_GPIOAEN |
						RCC_AHB1ENR_GPIOBEN |
						RCC_AHB1ENR_GPIOCEN |
						RCC_AHB1ENR_GPIODEN
					)
					;
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN	| 	//Enable DMA1
					RCC_AHB1ENR_DMA2EN		//Enable DMA2
					;
	//Enable ADC (Each ADC for one channel of RGB VGA)
	//RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
	//RCC->APB2ENR |= RCC_APB2ENR_ADC2EN;
	//RCC->APB2ENR |= RCC_APB2ENR_ADC3EN;
	//ENABLE DAC (LATER...!!!)
	//Enable TIM1
	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
	//Enable TIM2
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	//Enable TIM3
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	//Enable FMPI2C1
	RCC->APB1ENR |= RCC_APB1ENR_FMPI2C1EN;
	//Enable UART1
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	//Enable I2S
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
	//Enable SPDIF
	RCC->DCKCFGR2 &= ~RCC_DCKCFGR2_SPDIFRXSEL;	//set the SPDIF clock source as PLL_R
	RCC->APB1ENR |= RCC_APB1ENR_SPDIFRXEN;
}

/*			General-purpose I/Os								*/
static void _init_GPIO( void )
{
	/*		MODER REGs		*/
	GPIOA->MODER |= GPIO_MODER_MODER4_0  |							//GPO - THIS MAY CHANGE TO ANALOG!
					GPIO_MODER_MODER6_0  |							//GPO
					GPIO_MODER_MODER0_0  | GPIO_MODER_MODER0_1 |  	//ADC - IN0
					GPIO_MODER_MODER1_0  | GPIO_MODER_MODER1_1 |  	//ADC - IN1
					GPIO_MODER_MODER2_0  | GPIO_MODER_MODER2_1 |  	//ADC - IN2
					GPIO_MODER_MODER9_1  |							//UART1_TX
					GPIO_MODER_MODER10_1 |							//UART1_RX
					GPIO_MODER_MODER3_1  |							//TIM2_CH4
					GPIO_MODER_MODER5_1  |							//TIM2_CH1
					GPIO_MODER_MODER15_1							//I2S1_WS
					;
	GPIOB->MODER |= GPIO_MODER_MODER13_0 |							//GPO
					GPIO_MODER_MODER14_0 |							//GPO
					GPIO_MODER_MODER15_0 |							//GPO
					GPIO_MODER_MODER3_1  |							//I2S1_CK
					GPIO_MODER_MODER5_1  |							//I2S1_SD
					GPIO_MODER_MODER0_1	 |							//TIM3_CH3_WS2812_DIN
					GPIO_MODER_MODER1_1  |							//TIM3_CH4
					GPIO_MODER_MODER7_1	 							//SPDIFRX
					;
	GPIOC->MODER |= GPIO_MODER_MODER0_0  |							//GPO
					GPIO_MODER_MODER8_0  |							//GPO
					GPIO_MODER_MODER9_0  |							//GPO
					GPIO_MODER_MODER4_1  |							//I2S1_
					GPIO_MODER_MODER6_1	 |							//FMPI2C1_SCL
					GPIO_MODER_MODER7_1								//FMPI2C1_SDA
					;
	GPIOD->MODER |= GPIO_MODER_MODER2_0;							//GPO

	/*		OSPEEDR	REGs	*/
	GPIOA->OSPEEDR |= 	GPIO_OSPEEDR_OSPEED3_0	|					//TIM2_CH4
						GPIO_OSPEEDR_OSPEED5_0	|					//TIM2_CH1
						GPIO_OSPEEDR_OSPEED15_0						//I2S1_WS
						;
	GPIOB->OSPEEDR |=	GPIO_OSPEEDR_OSPEED3_0	|					//I2S1_CK
						GPIO_OSPEEDR_OSPEED5_0	|					//I2S1_SD
						GPIO_OSPEEDR_OSPEED0_0	|					//TIM3_CH3
						GPIO_OSPEEDR_OSPEED1_0	|					//TIM3_CH4
						GPIO_OSPEEDR_OSPEED7_0						//SPDIFRX
						;
	/*		AFRL REGs		*/
	GPIOA->AFR |= 	GPIO_AFRL_AFSEL3_0   |							//TIM2_CH4
					GPIO_AFRL_AFSEL5_0	 |							//TIM2_CH1
					(GPIO_AFRH_AFSEL9_0  | GPIO_AFRH_AFSEL9_1 | GPIO_AFRH_AFSEL9_2) |	//UART1_TX
					(GPIO_AFRH_AFSEL10_0 | GPIO_AFRH_AFSEL10_1 | GPIO_AFRH_AFSEL10_2) |	//UART1_RX
					(GPIO_AFRH_AFSEL15_0 | GPIO_AFRH_AFSEL15_2)		//I2S1_WS
					;
	GPIOB->AFR |= 	(GPIO_AFRL_AFSEL3_0 | GPIO_AFRL_AFSEL3_2) |		//I2S1_CK
					(GPIO_AFRL_AFSEL5_0 | GPIO_AFRL_AFSEL5_2) |		//I2S1_SD
					GPIO_AFRL_AFSEL0_1 	|							//TIM3_CH3
					GPIO_AFRL_AFSEL1_1 	|							//TIM3_CH4
					GPIO_AFRL_AFSEL7_3								//SPDIF_RX0
					;
	GPIOC->AFR |= 	GPIO_AFRL_AFSEL6_2	|							//FMPI2C1_SCL
					GPIO_AFRL_AFSEL7_2								//FMPI2C1_SDA
					;

	/*	RESET VALUE	*/
	GPIOA->ODR |= 0;
	GPIOB->ODR |= 0;
	GPIOC->ODR |= 0;
	GPIOD->ODR |= 0;
}

/*			Direct memory access controller						*/
static void _init_DMA( void )
{
	//set DMA1 - Stream5 for FMPI2C1_TX
	DMA1_Stream5->CR |= DMA_SxCR_CHSEL_1 |	// Selecting channel 2 for Stream 5 (ch2 is FMPI2C1_TX)
						DMA_SxCR_MINC	 |	// Memory address pointer will increase after each transfer.
						DMA_SxCR_DIR_0	 |	// Mem To Periph Direction.
						DMA_SxCR_PFCTRL		// Periph is controlling the flow
						;

	DMA1_Stream5->PAR = &FMPI2C1->TXDR;		// Assigning the TXD register of FMPI2C to DMA periph pointing address

	DMA2_Stream7->CR |= DMA_SxCR_CHSEL_2 |	// Selecting channel 4 for Stream 7 (ch4 is UART1_TX)
						DMA_SxCR_MINC	 |	// Memory address pointer will increase after each transfer.
						DMA_SxCR_DIR_0	 |	// Mem To Periph Direction.
						DMA_SxCR_PFCTRL	 	// Periph is controlling the flow
						;
	DMA2_Stream7->PAR = &USART1->DR;		// Assigning the Data register of USART1 to DMA periph pointer


	DMA2_Stream3->CR |= DMA_SxCR_CHSEL_0 | DMA_SxCR_CHSEL_1 |	// Selecting channel 3 for Stream 3 (ch3 is SPI1_TX)
						DMA_SxCR_MINC	 |	// Memory address pointer will increase after each transfer.
						DMA_SxCR_DIR_0	 |	// Mem To Periph Direction.
						DMA_SxCR_PFCTRL	 	// Periph is controlling the flow
						;
	DMA2_Stream3->PAR = &SPI1->DR;			// Assigning the Data register of SPI1 to DMA periph pointer.


	//Enable DMA for TIM3
	//Circular mode
	//DMA Address is DMAR of TIM3
	//1 Transfer, and interrupt should be activated...
	//The number of transfer would be checked manually!
}

static void DMAEnable( uint8_t channelNumber, uint8_t *data, uint32_t numOfTransfer)
{
	switch (channelNumber)
	{
	case DMA1_FMPI2C1_TX_EN:
		DMA1_Stream5->M0AR = *data;				// Pointing to mem address
		DMA1_Stream5->NDTR = numOfTransfer;		// Assigning the number of transfer
		DMA1_Stream5->CR |= DMA_SxCR_EN 	|	// Enabling Stream 5
							DMA_SxCR_TCIE		// Enable Transfer complete interrupt.
							;
		break;
	case DMA2_UART1_TX_EN:
		DMA2_Stream7->M0AR = *data;				// Pointing to mem address
		DMA2_Stream7->NDTR = numOfTransfer;		// Assigning the number of transfer
		DMA2_Stream7->CR |= DMA_SxCR_EN		|	// Enabling Stream 7
							DMA_SxCR_TCIE		// Enable Transfer complete interrupt.
							;
		break;
	case DMA2_SPI1_TX_EN:
		DMA2_Stream3->M0AR = *data;				// Pointing to mem address
		DMA2_Stream3->NDTR = numOfTransfer;		// Assigning the number of transfer
		DMA2_Stream3->CR |= DMA_SxCR_EN		|	// Enabling Stream 3
							DMA_SxCR_TCIE		// Enable Transfer complete interrupt.
							;
		break;
	default:
		break;

	}

}

void DMADisable( uint8_t channelNumber )
{
	switch (channelNumber)
	{
	case DMA1_FMPI2C1_TX_DIS:
		DMA1_Stream5->CR &= ~DMA_SxCR_EN;		// Disabling from Stream 5
		break;
	case DMA2_UART1_TX_DIS:
		DMA2_Stream7->CR &= ~DMA_SxCR_EN;		// Disabling from Stream 7
		break;
	case DMA2_SPI1_TX_DIS:
		DMA2_Stream3->CR &= ~DMA_SxCR_EN;		// Disabling from Stream 3
		break;
	default:
		break;

	}
}

/*			Analog-to-digital converter	1						*/
static void _init_ADC1( void )
{

}

/*			Analog-to-digital converter	2						*/
static void _init_ADC2( void )
{

}

/*			Analog-to-digital converter	3						*/
static void _init_ADC3( void )
{

}

/*			Digital-to-analog converter							*/
static void _init_DAC( void )
{

}

/*			Advanced-control timers (TIM1)						*/
static void _init_TIM1( void )
{

}

/*			General-purpose timers (TIMER2)						*/
static void _init_TIM2( void )
{

}

/*			General-purpose timers (TIMER3)						*/
static void _init_TIM3( void )
{
	TIM3->CCMR2 |= 	TIM_CCMR2_OC3M_0 | TIM_CCMR2_OC3M_1 |	// Toggle - OC1REF toggles when TIMx_CNT=TIMx_CCR1
					TIM_CCMR2_OC3PE							// Preload register on TIMx_CCR1 enabled.
					;
	TIM3->CCER |= TIM_CCER_CC3E;							// Capture/Compare 3 output enable.
	TIM3->ARR = 56;											// fclk = 45MHz, then 56 will make a signal with overall period of 1.25 usec.
	TIM3->CCR3 = 0;											// Initializing with 0 as compare value. Technically turning it off.
	TIM3->CR1 |= TIM_CR1_URS;								// Only counter overflow/underflow will generate an update request
	TIM3->DIER |= TIM_DIER_UIE;								// Update Interrupt Enable.
}

/*			Real-time clock										*/
static void _init_RTC( void )
{

}

/*			Inter-integrated circuit (I2C) interface			*/
static void _init_FMPI2C1( void )
{
	//Setting the clock to 400KHz
	FMPI2C1->TIMINGR |= (10 << 28) 	|				// PRESC = 10 --> 4000000MHz
						(1 << 20) 	| 				// SCLDEL = 1 --> 500  ns
						(1 << 16) 	| 				// SDADEL = 1 --> 250  ns
						(1 << 8) 	|				// SCLH = 1   --> 500  ns
						(4 << 0)					// SCLL = 4   --> 1250 ns
						;
	FMPI2C1->CR2 |= FMPI2C_CR2_RELOAD;				// The transfer is not completed after the NBYTES data transfer
	FMPI2C1->CR2 |= (1 << FMPI2C_CR2_NBYTES_Pos);	// Setting the no. of bytes to be transfered 1 that each byte there would be a tick to get new value from DMA.
	//Enable interrupts
	FMPI2C1->CR1 |= FMPI2C_CR1_TCIE 	| 			// Enable Transfer complete interrupt
					FMPI2C_CR1_ERRIE 	| 			// Enable Error flags interrupt
					FMPI2C_CR1_NACKIE				// Enable NACK interrupt
					;
	FMPI2C1->CR1 |= FMPI2C_CR1_PE;					// Enable I2C
}

/*			Universal asynchronous receiver transmitter			*/
static void _init_UART1( void )
{
	USART1->BRR |= (0X16 << 4);			// Configuring the baud rate for 250000 => ((176MHz/2) / (16* 22(USART_DIV)))
	USART1->CR2 |= USART_CR2_STOP_1;	// Selecting 2 stop bits
	USART1->CR3 |= USART_CR3_DMAT;		// Enabling DMA for Transmitter
	USART1->CR1 |= USART_CR1_TE;		// Enabling the Transmitter
}

/*			Inter-IC sound										*/
static void _init_I2S( void )
{
	//audio freq. sample =
	//Master mode
	//Data format = 16-bit right justified
	//Packet frame = ?
	//Clock polarity = ?
}

/*			SPDIF receiver interface							*/
static void _init_SPDIF( void )
{
	//we need DMA
	/*
	 * 	Connect the SPDIFRX_IN input to an external interrupt event block in order to detect
		transitions of SPDIFRX_IN line. When activity is detected, then SPDIFRXEN can be
		set to 01 or 11.
	 */
}



/***************************************************************************************************/
/**********************************                             ************************************/
/********************                     END OF THE CODE                         ******************/
/**********************************                             ************************************/
/***************************************************************************************************/

/*                                   GLOBAL FUNCTION TEMPLATE                                      */

/****************************************************************************************************
*   @Brief Description:
*   Function Status: 	DRAFT   (DRAFT , PRILIMINARY, CHECKED, RELEASED)
*
*	************************************************************************************************
*	Function Name:
*	Function Scope:			Global
*	Function Parameters:
*	Function Return Type:
*	************************************************************************************************
*	@Detailed Description: (Do numbering and tag the number to each part of code)
*	(1)
*	(2)
*	(3)
*	.
*	.
*	.
*	************************************************************************************************
*	Revision History (Description, author, date: yyyy/mm/dd)
*
****************************************************************************************************/

/*                               STATIC/LOCAL FUNCTION TEMPLATE                                    */

/****************************************************************************************************
*   @Brief Description:
*   Function Status: 	DRAFT   (DRAFT , PRELIMINARY, CHECKED, RELEASED)
*
*	************************************************************************************************
*	Function Name:
*	Function Scope:         Local(static)
*	Function Parameters:
*	Function Return Type:
*	************************************************************************************************
*	@Detailed Description: (Do numbering and tag the number to each part of code)
*	(1)
*	(2)
*	(3)
*	.
*	.
*	.
*	************************************************************************************************
*	Revision History (Description (author, date: yyyy/mm/dd))
*
****************************************************************************************************/

/************************************     END OF THE FILE      *************************************/
