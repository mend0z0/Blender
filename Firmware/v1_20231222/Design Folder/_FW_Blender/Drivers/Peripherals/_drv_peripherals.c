/****************************************************************************************************
*
*   Project Name:       Blender
*   @Brief Description: Blending audio spec with light spectrum
*   File Status:	    PRELIMINARY   (DRAFT , PRELIMINARY, CHECKED, RELEASED)
*
*	File Name:	_drv_pheripherals.c
*	Version:	01
*	Revision:	01
*	Date:		2024/02/19
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
****************************   CONST VARIABLES DECLARATION    ***************************************
*****************************************************************************************************/
const uint32_t SysTickInputClock = (90000000 / 8);			// The AHB output is 90MHz and there is an extra 1/8 multiplier.
const uint16_t DAC_CONSTANT = 40U;							// The 100% output actual constant is 40.96, but I kept it 40 for not overflowing.
static const TickType_t semaphoreWatiTime10ms = pdMS_TO_TICKS(10);	// 10msec delay for semaphore to get created, if it didn't happen immediately!

/****************************************************************************************************
****************************   GLOB. VARIABLES DECLARATION    ***************************************
*****************************************************************************************************/
SemaphoreHandle_t FMPI2CBinarySemaphore = pdFALSE;			//
SemaphoreHandle_t USART1BinarySemaphore = pdFALSE;			//

/****************************************************************************************************
***********************     STATIC/LOCAL FUNCTIONS DECLARATION      *********************************
*****************************************************************************************************/
/*			SysTick Timer	 									*/
static void _init_SysTick( void );

/*			Power controller 									*/
static void _init_PWR( void );

/*			Reset and clock control 							*/
static void _init_RCC( void );

/*			General-purpose I/Os								*/
static void _init_GPIO( void );

/*			Direct memory access controller						*/
static void _init_DMA( void );
static void DMAEnable( uint8_t channelNumber, uint8_t *data, uint32_t numOfTransfer);
static void DMADisable( uint8_t channelNumber );

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

/****************************************************************************************************
*   @Brief Description:	This functions holds all the functions that will initialize all the peripherals registers.
*   Function Status: 	PRILIMINARY   (DRAFT , PRILIMINARY, CHECKED, RELEASED)
*
*	************************************************************************************************
*	Function Name:			_init_Peripherals()
*	Function Scope:			Global
*	Function Parameters:	void
*	Function Return Type:	void
*	************************************************************************************************
*	@Detailed Description: (Do numbering and tag the number to each part of code)
*	(1) Init all the required peripherals.
*	************************************************************************************************
*	Revision History (Description, author, date: yyyy/mm/dd)
*
****************************************************************************************************/
void _init_Peripherals( void ){
	//_init_PWR();
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
	_init_SysTick();
}

/*			General-purpose I/Os								*/

/*			Digital-to-analog converter							*/

/****************************************************************************************************
*   @Brief Description:	Updating the DAC output value based on percents not real output voltage.
*   Function Status: 	PRILIMINARY   (DRAFT , PRILIMINARY, CHECKED, RELEASED)
*
*	************************************************************************************************
*	Function Name:			DACUpdate()
*	Function Scope:			Global
*	Function Parameters:	uint8_t dacValue (It's a percent from 0% to 100%)
*	Function Return Type:	void
*	************************************************************************************************
*	@Detailed Description: (Do numbering and tag the number to each part of code)
*	(1) Receiving the input percentage and converting into a 12bits value
*	(2) Uploading the new value into DAC1 register.
*	(3) Update new value and this bit will clear by hardware (one APB1 clock cycle later) once
*		the DAC_DHR1 register value has been loaded into the DAC_DOR1 register
*	************************************************************************************************
*	Revision History (Description, author, date: yyyy/mm/dd)
*
****************************************************************************************************/
void DACUpdate( uint8_t dacValue)
{
	__IO uint16_t tempDACValue = dacValue * DAC_CONSTANT;	// (1)
	DAC1->DHR12R1 = tempDACValue;							// (2)
	DAC1->SWTRIGR |= DAC_SWTRIGR_SWTRIG1;					// (3)

	//No interrupt, it'll work probably :D
	//I can read the DAC1->DOR1 to make sure it's been registered, but maybe later...
}

/*			Analog-to-digital converter							*/

/*			Advanced-control timers (TIM1)						*/

/*			General-purpose timers (TIM3)						*/

/****************************************************************************************************
*   @Brief Description:	Preset the TIM3 counter register and enabling the timer
*   Function Status: 	PRILIMINARY   (DRAFT , PRILIMINARY, CHECKED, RELEASED)
*
*	************************************************************************************************
*	Function Name:			TIM3Enable()
*	Function Scope:			Global
*	Function Parameters:	void
*	Function Return Type:	void
*	************************************************************************************************
*	@Detailed Description: (Do numbering and tag the number to each part of code)
*	(1) Set the counter to 0
*	(2) Enable the timer
*	************************************************************************************************
*	Revision History (Description, author, date: yyyy/mm/dd)
*
****************************************************************************************************/
void TIM3Enable( void )
{
	TIM3->CNT = 0;				// (1)
	TIM3->CR1 |= TIM_CR1_CEN;	// (2)
}

/****************************************************************************************************
*   @Brief Description: Disable Timer3
*   Function Status: 	PRILIMINARY   (DRAFT , PRILIMINARY, CHECKED, RELEASED)
*
*	************************************************************************************************
*	Function Name:			TIM3Disable()
*	Function Scope:			Global
*	Function Parameters:	void
*	Function Return Type:	void
*	************************************************************************************************
*	@Detailed Description: (Do numbering and tag the number to each part of code)
*	(1) Disable Timer 3
*	************************************************************************************************
*	Revision History (Description, author, date: yyyy/mm/dd)
*
****************************************************************************************************/
void TIM3Disable( void )
{
	TIM3->CR1 &= ~TIM_CR1_CEN;	// (1)
}

/****************************************************************************************************
*   @Brief Description:	Update CCR3 register of Timer 3
*   Function Status: 	PRILIMINARY   (DRAFT , PRILIMINARY, CHECKED, RELEASED)
*
*	************************************************************************************************
*	Function Name:			TIM3UpdateCCR3()
*	Function Scope:			Global
*	Function Parameters:	uint32_t ccrValue
*	Function Return Type:	void
*	************************************************************************************************
*	@Detailed Description: (Do numbering and tag the number to each part of code)
*	(1)	Update TIM3
*	************************************************************************************************
*	Revision History (Description, author, date: yyyy/mm/dd)
*
****************************************************************************************************/
void TIM3UpdateCCR3( uint32_t ccrValue )
{
	TIM3->CCR3 = ccrValue;	// (1)
}

/*			Real-time clock										*/

/*			Inter-integrated circuit (I2C) interface			*/

/****************************************************************************************************
*   @Brief Description:	Sending an array of data over FMPI2C1
*   Function Status: 	PRILIMINARY   (DRAFT , PRILIMINARY, CHECKED, RELEASED)
*
*	************************************************************************************************
*	Function Name:			FMPI2C1DataTx()
*	Function Scope:			Global
*	Function Parameters:	uint8_t slaveAddr, uint8_t *data, uint32_t buffSize
*	Function Return Type:	int32_t
*	************************************************************************************************
*	@Detailed Description: (Do numbering and tag the number to each part of code)
*	(1) Creating semaphore for FMPI2C
*	(2) If NULL is returned, then the semaphore cannot be created
*	(3) Updating the status with pdFALSE (0)
*	(4) Init DMA with data address and number of transfer and then enable it.
*	(5) Enable Transmit interrupt (it's mainly for confirming the sent addr)
*	(6) Enable NACK interrupt.
*	(7) Set Slave Address
*	(8) Set transfer direction to 0 to write on slave.
*	(9) Sending an start on line
*	(10) At this point, we'll wait until we give the semaphore back from ISR
*	(11) Once we've received the semaphore from ISR we'll check if there was an error or completion of transmitted data.
*	(12) Checking if the semaphore has given back from NACK (error) ISR and if yes
*	(13) Clearing the NACK flag.
*	(14) Returning error
*	(15) Otherwise if the semaphore has given back from DMA transfer finished flag then
*	(16) Clearing the flag by writing a one.
*	(17) Updating the status variable with the number of data that has been sent out.
*	(18) Otherwise other errors has happened that we received the semaphore so we set the status as unknown error (-3)
*	(19) Returning the status
*	************************************************************************************************
*	Revision History (Description, author, date: yyyy/mm/dd)
*
****************************************************************************************************/
int32_t FMPI2C1DataTx( uint8_t slaveAddr, uint8_t *data, uint32_t buffSize)
{
	__IO int32_t status = 0;

	FMPI2CBinarySemaphore = xSemaphoreCreateBinary();					// (1)

	if(FMPI2CBinarySemaphore == NULL){									// (2)
		status = pdFALSE;												// (3)
	}
	else{
		DMAEnable( DMA1_FMPI2C1_TX_EN, *data, buffSize);				// (4)
		FMPI2C1->CR1 |= FMPI2C_CR1_TXIE 	| 							// (5)
						FMPI2C_CR1_NACKIE								// (6)
						;
		FMPI2C1->CR2 |= ((slaveAddr & 0X7F) << 1);						// (7)
		FMPI2C1->CR2 &= ~FMPI2C_CR2_RD_WRN;								// (8)
		FMPI2C1->CR2 |=	FMPI2C_CR2_START;								// (9)

		xSemaphoreTake( FMPI2CBinarySemaphore, semaphoreWatiTime10ms );		// (10)

		DMADisable( DMA1_FMPI2C1_TX_DIS );								// (11)

		if((FMPI2C1->ISR & FMPI2C_ISR_NACKF) == FMPI2C_ISR_NACKF)		// (12)
		{
			FMPI2C1->ICR |= FMPI2C_ICR_NACKCF;		 					// (13)
			status = FMPI2C_ERROR_NACK;				 					// (14)
		}
		else if((DMA1->HISR & DMA_HISR_TCIF5) == DMA_HISR_TCIF5)		// (15)
		{
			DMA1->HIFCR |= DMA_HIFCR_CTCIF5;							// (16)
			status = (int32_t)buffSize;									// (17)
		}
		else{
			status = FMPI2C_ERROR_UNKNOWN;								// (18)
		}
	}

	return status;														// (19)
}

/*			Universal asynchronous receiver transmitter			*/

/****************************************************************************************************
*   @Brief Description: Sending out an array of data over UART
*   Function Status: 	PRILIMINARY   (DRAFT , PRILIMINARY, CHECKED, RELEASED)
*
*************************************************************************************************
*	Function Name:			UART1DataTx()
*	Function Scope:			Global
*	Function Parameters:	uint8_t *data, uint32_t buffSize
*	Function Return Type:	int32_t
*************************************************************************************************
*	@Detailed Description: (Do numbering and tag the number to each part of code)
*	(2) If NULL is returned, then the semaphore cannot be created then
*	(3) Updating the status value with pdFALSE (0)
*	(4) Init DMA with data address and number of transfer and then enable it.
*	(5) Enabling the transmission complete interrupt
*	(6) Enabling the UART peripheral.
*	(7) Creating a semaphore to hold the code here and send the data from DMA to USART
*	(8) After the Semaphore has been given we should disable DMA
*	(9) Checking if the TC has been raised, in that case the transfer has been completed.
*	(10) Clearing the TC flag.
*	(11) Updating the status variable with the number of data that has been sent as a successful transfer.
*	(12) Otherwise if we received the semaphore as a result of other ISR, we'll set the status to pdFALSE (0)
*	(13) Disable the UART peripheral.
*	(14) Return the status.
*************************************************************************************************
*	Revision History (Description, author, date: yyyy/mm/dd)
*
****************************************************************************************************/
int32_t UART1DataTx( uint8_t *data, uint32_t buffSize)
{
  __IO int32_t status = 0;


  DMAEnable( DMA2_UART1_TX_EN, *data, buffSize);			// (4)
  USART1->CR1 |= USART_CR1_TCIE;					// (5)
  USART1->CR1 |= USART_CR1_UE;						// (6)

  while(1)
    {
      if(xSemaphoreTake( USART1BinarySemaphore, portMAX_DELAY))		// (7)
	{
	  DMADisable( DMA2_UART1_TX_DIS );				// (8)
	  if((USART1->SR & USART_SR_TC) == USART_SR_TC)			// (9)
	    {
	      USART1->SR &= ~USART_SR_TC;				// (10)
	      status = (int32_t)buffSize;				// (11)
	    }
	  else
	    {
	      status = pdFALSE;						// (12)
	    }
	}
    }
  USART1->CR1 &= ~USART_CR1_UE;						// (13)
  return status;							// (14)
}

/*			Inter-IC sound										*/

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
/*			SysTick Timer	 									*/

/****************************************************************************************************
*   @Brief Description: Init Systick peripheral with clock value
*   Function Status: 	PRELIMINARY   (DRAFT , PRELIMINARY, CHECKED, RELEASED)
*
*	************************************************************************************************
*	Function Name:			_init_SysTick()
*	Function Scope:         Local(static)
*	Function Parameters:	void
*	Function Return Type:	void
*	************************************************************************************************
*	@Detailed Description: (Do numbering and tag the number to each part of code)
*	(1) Configuring the Systick timer.
*	************************************************************************************************
*	Revision History (Description (author, date: yyyy/mm/dd))
*
****************************************************************************************************/
static void _init_SysTick( void )
{
	SysTick_Config(SysTickInputClock / 1000);		// (1)
}

/*			Power controller 									*/

/****************************************************************************************************
*   @Brief Description:	Configure the power peripheral.
*   Function Status: 	PRELIMINARY   (DRAFT , PRELIMINARY, CHECKED, RELEASED)
*
*	************************************************************************************************
*	Function Name:			_init_PWR()
*	Function Scope:         Local(static)
*	Function Parameters:	void
*	Function Return Type:	void
*	************************************************************************************************
*	@Detailed Description: (Do numbering and tag the number to each part of code)
*	(1) PROBABLY i NEED TO ENABLE THE CLOCK...????!?!?!
*	(2) Selecting Scale mode 1
*	(3) Enabling the over drive
*	(4) Waiting for the Over-drive mode ready flag to set
*	(5) Enabling over drive switch to connect the 1.2 to domain.
*	(6) Waiting for the Over-drive mode switching ready flag to set
*	************************************************************************************************
*	Revision History (Description (author, date: yyyy/mm/dd))
*
****************************************************************************************************/
static void _init_PWR( void )
{
	//RCC->APB1ENR |= RCC_APB1ENR_PWREN;		// (1)
	PWR->CR |= PWR_CR_VOS;						// (2)
	PWR->CR |= PWR_CR_ODEN;						// (3)
	while((PWR->CSR & PWR_CSR_ODRDY) == 0)		// (4)
	{
		//maybe adding a time-out later
	}
	PWR->CR |= PWR_CR_ODSWEN;					// (5)
	while((PWR->CSR & PWR_CSR_ODSWRDY) == 0)	// (6)
	{
		//maybe adding a time-out later
	}
}

/*			Reset and clock control 							*/

/****************************************************************************************************
*   @Brief Description: Initialize and config the RCC peripheral.
*   Function Status: 	PRELIMINARY   (DRAFT , PRELIMINARY, CHECKED, RELEASED)
*
*	************************************************************************************************
*	Function Name:			_init_RCC()
*	Function Scope:         Local(static)
*	Function Parameters:	void
*	Function Return Type:	void
*	************************************************************************************************
*	@Detailed Description: (Do numbering and tag the number to each part of code)
*	(1) Enabling the external clock
*	(2) Waiting for the HSE clock ready flag to set
*	(3) Set the PLL clock source as HSE;
*	(4)		HSE FREQ. = 16MHz
		Caution: The software has to set these bits correctly to ensure that the VCO output
		 			frequency is between 100 and 432 MHz.
			f(VCO clock) = f(PLL clock input) × (PLLN / PLLM)
			f(VCO clock) = HSE (16MHz) * (PLLN (180) / PLLM (8)) = 16 * 22.5 ~= 360 MHz
		 	f(PLL general clock output) = f(VCO clock) / PLLP
		 	f(PLL general clock output) = 360 (352) / 2 ~= 180MHz (176MHz)
*	(5) PPLR Division factor to 2 so the SPDIF and I2S clock is 90MHz.
*	(6) Enable the main PLL
*	(7) Waiting for the main PLL locks
*	(8) Set the clock source as PLLCLK
*	(9) Making sure the clock source is PLLCLK
*	(10) Configure AHB prescaler, divide by 2 ~= 90MHz
*	(11) Configure APB1 prescaler, divide by 4 ~= 45MHz
*	(12) Configure APB2 prescaler, divide by 2 ~= 90MHz
*	(13) Enable GPIO A
*	(14) Enable GPIO B
*	(15) Enable GPIO C
*	(16) Enable GPIO D
*	(17) Enable DMA1
*	(18) Enable DMA2
*	(19) Enable ADC (Each ADC for one channel of RGB VGA)
*	(20) Enable ADC (Each ADC for one channel of RGB VGA)
*	(21) Enable ADC (Each ADC for one channel of RGB VGA)
*	(22) ENABLE DAC
*	(23) Enable TIM1
*	(24) Enable TIM2
*	(25) Enable TIM3
*	(26) Enable FMPI2C1
*	(27) Enable UART1
*	(28) Enable I2S
*	(29) Set the SPDIF clock source as PLL_R
*	(30) Enable SPDIF
*	************************************************************************************************
*	Revision History (Description (author, date: yyyy/mm/dd))
*
****************************************************************************************************/
static void _init_RCC( void )
{
	////--------------Enabling the HSE
	RCC->CR |= RCC_CR_HSEON;								// (1)
	while((RCC->CR & RCC_CR_HSERDY) == 0)					// (2)
	{
		//maybe add a time out later
	}

	////--------------Configuration for PLLs
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC;						// (3)

	RCC->PLLCFGR |= ((	RCC_PLLCFGR_PLLN_2 	|				// (4)
						RCC_PLLCFGR_PLLN_4 	|
						RCC_PLLCFGR_PLLN_5 	|
						RCC_PLLCFGR_PLLN_7) |
						RCC_PLLCFGR_PLLM_3);

	RCC->PLLCFGR |= RCC_PLLCFGR_PLLR_1;						// (5)

	RCC->CR |= RCC_CR_PLLON;								// (6)
	while((RCC->CR & RCC_CR_PLLRDY) == 0)					// (7)
	{
		//maybe add a time out later
	}

	RCC->CFGR |= RCC_CFGR_SW_1;								// (8)
	while((RCC->CFGR & RCC_CFGR_SWS_1) != RCC_CFGR_SWS_1)	// (9)
	{
		//maybe add a time out later
	}

	RCC->CFGR |= RCC_CFGR_HPRE_3;							// (10)
	RCC->CFGR |= RCC_CFGR_PPRE1_0 | RCC_CFGR_PPRE1_2; 		// (11)
	RCC->CFGR |= RCC_CFGR_PPRE2_2;							// (12)


	RCC->AHB1ENR |= (	RCC_AHB1ENR_GPIOAEN |				// (13)
						RCC_AHB1ENR_GPIOBEN |				// (14)
						RCC_AHB1ENR_GPIOCEN |				// (15)
						RCC_AHB1ENR_GPIODEN					// (16)
					);
	;
	RCC->AHB1ENR |= (	RCC_AHB1ENR_DMA1EN	| 				// (17)
					 	RCC_AHB1ENR_DMA2EN					// (18)
					);
	//RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;					// (19)
	//RCC->APB2ENR |= RCC_APB2ENR_ADC2EN;					// (20)
	//RCC->APB2ENR |= RCC_APB2ENR_ADC3EN;					// (21)
	RCC->APB1ENR |= RCC_APB1ENR_DACEN;						// (22)
	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;						// (23)
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;						// (24)
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;						// (25)
	RCC->APB1ENR |= RCC_APB1ENR_FMPI2C1EN;					// (26)
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;					// (27)
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;						// (28)
	RCC->DCKCFGR2 &= ~RCC_DCKCFGR2_SPDIFRXSEL;				// (29)
	RCC->APB1ENR |= RCC_APB1ENR_SPDIFRXEN;					// (30)
}

/*			General-purpose I/Os								*/

/****************************************************************************************************
*   @Brief Description:	Configure the pins of each GPIO port.
*   Function Status: 	PRELIMINARY   (DRAFT , , CHECKED, RELEASED)
*
*	************************************************************************************************
*	Function Name:			_init_GPIO()
*	Function Scope:         Local(static)
*	Function Parameters:	void
*	Function Return Type:	void
*	************************************************************************************************
*	@Detailed Description: (Do numbering and tag the number to each part of code)
*	(1) Set GPIOA.4 as DAC_OUT1
*	(2) Set GPIOA.6 as GPO
*	(3) Set GPIOA.0 as ADC - IN0
*	(4) Set GPIOA.1 as ADC - IN1
*	(5) Set GPIOA.2 as ADC - IN2
*	(6) Set GPIOA.9 as UART1_TX
*	(7) Set GPIOA.10 as UART1_RX
*	(8) Set GPIOA.3 as TIM2_CH4
*	(9) Set GPIOA.5 as TIM2_CH1
*	(10) Set GPIOA.15 as I2S1_WS
*
*	(11) Set GPIOB.13 as GPO
*	(12) Set GPIOB.14 as GPO
*	(13) Set GPIOB.15 as GPO
*	(14) Set GPIOB.3 as I2S1_CK
*	(15) Set GPIOB.5 as I2S1_SD
*	(16) Set GPIOB.0 as TIM3_CH3_WS2812_DIN
*	(17) Set GPIOB.1 as TIM3_CH4
*	(18) Set GPIOB.7 as SPDIFRX
*
*	(19) Set GPIOC.0 as GPO
*	(20) Set GPIOC.8 as GPO
*	(21) Set GPIOC.9 as GPO
*	(22) Set GPIOC.4 as I2S1
*	(23) Set GPIOC.6 as FMPI2C1_SCL
*	(24) Set GPIOC.7 as FMPI2C1_SDA
*
*	(25) Set GPIOD.2 as GPO
*
*	(26) TIM2_CH4 pin speed is set to Medium speed
*	(27) TIM2_CH1 pin speed is set to Medium speed
*	(28) I2S1_WS pin speed is set to Medium speed
*
*	(29) I2S1_CK pin speed is set to Medium speed
*	(30) I2S1_SD pin speed is set to Medium speed
*	(31) TIM3_CH3 pin speed is set to Medium speed
*	(32) TIM3_CH4 pin speed is set to Medium speed
*	(33) SPDIFRX pin speed is set to Medium speed
*
*	(34) Set GPIOA.3 alternate function to TIM2_CH4
*	(35) Set GPIOA.5 alternate function to TIM2_CH1
*	(36) Set GPIOA.9 alternate function to UART1_TX
*	(37) Set GPIOA.10 alternate function to UART1_RX
*	(38) Set GPIOA.15 alternate function to I2S1_WS
*
*	(39) Set GPIOB.3 alternate function to I2S1_CK
*	(40) Set GPIOB.5 alternate function to I2S1_SD
*	(41) Set GPIOB.0 alternate function to TIM3_CH3
*	(42) Set GPIOB.1 alternate function to TIM3_CH4
*	(43) Set GPIOB.7 alternate function to SPDIF_RX0
*
*	(44) Set GPIOC.6 alternate function to FMPI2C1_SCL
*	(45) Set GPIOC.7 alternate function to FMPI2C1_SDA
*
*	(46)
*	(47)
*	(48)
*	(49)
*	************************************************************************************************
*	Revision History (Description (author, date: yyyy/mm/dd))
*
****************************************************************************************************/
static void _init_GPIO( void )
{
	/*		MODER REGs		*/
	GPIOA->MODER |= (	GPIO_MODER_MODER4_0  | GPIO_MODER_MODER4_1 |							// (1)
						GPIO_MODER_MODER6_0  |													// (2)
						GPIO_MODER_MODER0_0  | GPIO_MODER_MODER0_1 |  							// (3)
						GPIO_MODER_MODER1_0  | GPIO_MODER_MODER1_1 |  							// (4)
						GPIO_MODER_MODER2_0  | GPIO_MODER_MODER2_1 |  							// (5)
						GPIO_MODER_MODER9_1  |													// (6)
						GPIO_MODER_MODER10_1 |													// (7)
						GPIO_MODER_MODER3_1  |													// (8)
						GPIO_MODER_MODER5_1  |													// (9)
						GPIO_MODER_MODER15_1													// (10)
					);

	GPIOB->MODER |= (	GPIO_MODER_MODER13_0 |													// (11)
						GPIO_MODER_MODER14_0 |													// (12)
						GPIO_MODER_MODER15_0 |													// (13)
						GPIO_MODER_MODER3_1  |													// (14)
						GPIO_MODER_MODER5_1  |													// (15)
						GPIO_MODER_MODER0_1	 |													// (16)
						GPIO_MODER_MODER1_1  |													// (17)
						GPIO_MODER_MODER7_1	 													// (18)
					);

	GPIOC->MODER |= (	GPIO_MODER_MODER0_0  |													// (19)
						GPIO_MODER_MODER8_0  |													// (20)
						GPIO_MODER_MODER9_0  |													// (21)
						GPIO_MODER_MODER4_1  |													// (22)
						GPIO_MODER_MODER6_1	 |													// (23)
						GPIO_MODER_MODER7_1														// (24)
					);

	GPIOD->MODER |= GPIO_MODER_MODER2_0;														// (25)

	/*		OSPEEDR	REGs	*/
	GPIOA->OSPEEDR |=	(	GPIO_OSPEEDR_OSPEED3_0	|											// (26)
							GPIO_OSPEEDR_OSPEED5_0	|											// (27)
							GPIO_OSPEEDR_OSPEED15_0												// (28)
						);

	GPIOB->OSPEEDR |=	(	GPIO_OSPEEDR_OSPEED3_0	|											// (29)
							GPIO_OSPEEDR_OSPEED5_0	|											// (30)
							GPIO_OSPEEDR_OSPEED0_0	|											// (31)
							GPIO_OSPEEDR_OSPEED1_0	|											// (32)
							GPIO_OSPEEDR_OSPEED7_0												// (33)
						);
	/*		AFRL REGs		*/
	GPIOA->AFR[0] |= 	(	GPIO_AFRL_AFSEL3_0   |												// (34)
							GPIO_AFRL_AFSEL5_0	 												// (35)
						);

	GPIOA->AFR[1] |= 	(	(GPIO_AFRH_AFSEL9_0  | GPIO_AFRH_AFSEL9_1 | GPIO_AFRH_AFSEL9_2) |	// (36)
							(GPIO_AFRH_AFSEL10_0 | GPIO_AFRH_AFSEL10_1 | GPIO_AFRH_AFSEL10_2) |	// (37)
							(GPIO_AFRH_AFSEL15_0 | GPIO_AFRH_AFSEL15_2)							// (38)
						);

	GPIOB->AFR[0] |= 	(	(GPIO_AFRL_AFSEL3_0 | GPIO_AFRL_AFSEL3_2) |							// (39)
							(GPIO_AFRL_AFSEL5_0 | GPIO_AFRL_AFSEL5_2) |							// (40)
							GPIO_AFRL_AFSEL0_1 	|												// (41)
							GPIO_AFRL_AFSEL1_1 	|												// (42)
							GPIO_AFRL_AFSEL7_3													// (43)
						);

	GPIOC->AFR[0] |= 	(	GPIO_AFRL_AFSEL6_2	|												// (44)
							GPIO_AFRL_AFSEL7_2													// (45)
						);

	/*	RESET VALUE	*/
	GPIOA->ODR = 0;																				// (46)
	GPIOB->ODR = 0;																				// (47)
	GPIOC->ODR = 0;																				// (48)
	GPIOD->ODR = 0;																				// (49)
}

/*			Direct memory access controller						*/

/****************************************************************************************************
*   @Brief Description: Initialize DMA peripheral
*   Function Status: 	PRELIMINARY   (DRAFT , PRELIMINARY, CHECKED, RELEASED)
*
*	************************************************************************************************
*	Function Name:			_init_DMA()
*	Function Scope:         Local(static)
*	Function Parameters:	void
*	Function Return Type:	void
*	************************************************************************************************
*	@Detailed Description: (Do numbering and tag the number to each part of code)
*	(1) set DMA1 - Stream5 for FMPI2C1_TX
*	(2) Selecting channel 2 for Stream 5 (ch2 is FMPI2C1_TX)
*	(3) Memory address pointer will increase after each transfer.
*	(4) Mem To Periph Direction.
*	(5) Periph is controlling the flow
*	(6) Assigning the TXD register of FMPI2C to DMA periph pointing address
*	(7) Selecting channel 4 for Stream 7 (ch4 is UART1_TX)
*	(8) Memory address pointer will increase after each transfer.
*	(9) Mem To Periph Direction.
*	(10) Periph is controlling the flow
*	(11) Assigning the Data register of USART1 to DMA periph pointer
*	(12) Selecting channel 3 for Stream 3 (ch3 is SPI1_TX)
*	(13) Memory address pointer will increase after each transfer.
*	(14) Mem To Periph Direction.
*	(15) Periph is controlling the flow
*	(16) Assigning the Data register of SPI1 to DMA periph pointer.
*	************************************************************************************************
*	Revision History (Description (author, date: yyyy/mm/dd))
*
****************************************************************************************************/
static void _init_DMA( void )
{
	// (1)
	DMA1_Stream5->CR |= (	DMA_SxCR_CHSEL_1 |						// (2)
							DMA_SxCR_MINC	 |						// (3)
							DMA_SxCR_DIR_0	 |						// (4)
							DMA_SxCR_PFCTRL							// (5)
						);

	DMA1_Stream5->PAR = &FMPI2C1->TXDR;								// (6)

	DMA2_Stream7->CR |= (	DMA_SxCR_CHSEL_2 |						// (7)
							DMA_SxCR_MINC	 |						// (8)
							DMA_SxCR_DIR_0	 |						// (9)
							DMA_SxCR_PFCTRL	 						// (10)
						);

	DMA2_Stream7->PAR = &USART1->DR;								// (11)


	DMA2_Stream3->CR |= (	DMA_SxCR_CHSEL_0 | DMA_SxCR_CHSEL_1 |	// (12)
							DMA_SxCR_MINC	 |						// (13)
							DMA_SxCR_DIR_0	 |						// (14)
							DMA_SxCR_PFCTRL	 						// (15)
						);

	DMA2_Stream3->PAR = &SPI1->DR;									// (16)
}

/****************************************************************************************************
*   @Brief Description: Enable DMA for specified channel and address DMA register the data address and set the number of transfers.
*   Function Status: 	PRELIMINARY   (DRAFT , PRELIMINARY, CHECKED, RELEASED)
*
*	************************************************************************************************
*	Function Name:			DMAEnable()
*	Function Scope:         Local(static)
*	Function Parameters:	uint8_t channelNumber, uint8_t *data, uint32_t numOfTransfer
*	Function Return Type:	void
*	************************************************************************************************
*	@Detailed Description: (Do numbering and tag the number to each part of code)
*	(1) Pointing to mem address
*	(2) Assigning the number of transfer
*	(3) Enabling Stream 5
*	(4) Enable Transfer complete interrupt.
*	(5) Pointing to mem address
*	(6) Assigning the number of transfer
*	(7) Enabling Stream 7
*	(8) Enable Transfer complete interrupt.
*	(9) Pointing to mem address
*	(10) Assigning the number of transfer
*	(11) Enabling Stream 3
*	(12) Enable Transfer complete interrupt.
*	************************************************************************************************
*	Revision History (Description (author, date: yyyy/mm/dd))
*
****************************************************************************************************/
static void DMAEnable( uint8_t channelNumber, uint8_t *data, uint32_t numOfTransfer)
{
	switch (channelNumber)
	{
	case DMA1_FMPI2C1_TX_EN:
		DMA1_Stream5->M0AR = *data;					// (1)
		DMA1_Stream5->NDTR = numOfTransfer;			// (2)
		DMA1_Stream5->CR |= (	DMA_SxCR_EN 	|	// (3)
								DMA_SxCR_TCIE		// (4)
							);
		break;
	case DMA2_UART1_TX_EN:
		DMA2_Stream7->M0AR = *data;					// (5)
		DMA2_Stream7->NDTR = numOfTransfer;			// (6)
		DMA2_Stream7->CR |= (	DMA_SxCR_EN		|	// (7)
								DMA_SxCR_TCIE		// (8)
							);
		break;
	case DMA2_SPI1_TX_EN:
		DMA2_Stream3->M0AR = *data;					// (9)
		DMA2_Stream3->NDTR = numOfTransfer;			// (10)
		DMA2_Stream3->CR |= (	DMA_SxCR_EN		|	// (11)
								DMA_SxCR_TCIE		// (12)
							);
		break;
	default:
		break;

	}

}

/****************************************************************************************************
*   @Brief Description: Disable the specified DMA channel.
*   Function Status: 	PRELIMINARY   (DRAFT , PRELIMINARY, CHECKED, RELEASED)
*
*	************************************************************************************************
*	Function Name:			DMADisable()
*	Function Scope:         Local(static)
*	Function Parameters:	uint8_t channelNumber
*	Function Return Type:	void
*	************************************************************************************************
*	@Detailed Description: (Do numbering and tag the number to each part of code)
*	(1) Disabling from Stream 5
*	(2) Disabling from Stream 7
*	(3) Disabling from Stream 3
*	************************************************************************************************
*	Revision History (Description (author, date: yyyy/mm/dd))
*
****************************************************************************************************/
static void DMADisable( uint8_t channelNumber )
{
	switch (channelNumber)
	{
	case DMA1_FMPI2C1_TX_DIS:
		DMA1_Stream5->CR &= ~DMA_SxCR_EN;		// (1)
		break;
	case DMA2_UART1_TX_DIS:
		DMA2_Stream7->CR &= ~DMA_SxCR_EN;		// (2)
		break;
	case DMA2_SPI1_TX_DIS:
		DMA2_Stream3->CR &= ~DMA_SxCR_EN;		// (3)
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

/****************************************************************************************************
*   @Brief Description: Initialize and turn the DAC on.
*   Function Status: 	PRELIMINARY   (DRAFT , PRELIMINARY, CHECKED, RELEASED)
*
*	************************************************************************************************
*	Function Name:			_init_DAC()
*	Function Scope:         Local(static)
*	Function Parameters:	void
*	Function Return Type:	void
*	************************************************************************************************
*	@Detailed Description: (Do numbering and tag the number to each part of code)
*	(1) Enabling DAC1
*	(2) Reset value
*	(3) Updating the output.
*	************************************************************************************************
*	Revision History (Description (author, date: yyyy/mm/dd))
*
****************************************************************************************************/
static void _init_DAC( void )
{
	//There is not much of configuration, but the DAC will trigger by software.
	DAC1->CR |= DAC_CR_EN1;					// (1)
	DAC1->DHR12R1 = 0;						// (2)
	DAC1->SWTRIGR |= DAC_SWTRIGR_SWTRIG1;	// (3)
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

/****************************************************************************************************
*   @Brief Description: Init TIM3
*   Function Status: 	PRELIMINARY   (DRAFT , PRELIMINARY, CHECKED, RELEASED)
*
*************************************************************************************************
*	Function Name:			_init_TIM3()
*	Function Scope:         Local(static)
*	Function Parameters:	void
*	Function Return Type:	static void
*************************************************************************************************
*	@Detailed Description: (Do numbering and tag the number to each part of code)
*	(1) Toggle - OC1REF toggles when TIMx_CNT=TIMx_CCR1
*	(2) Preload register on TIMx_CCR1 enabled.
*	(3) Capture/Compare 3 output enable.
*	(4) Fclk = 45MHz, then 56 will make a signal with overall period of 1.25 usec.
*	(5) Initializing with 0 as compare value. Technically turning it off.
*	(6) Only counter overflow/underflow will generate an update request
*	(7) Update Interrupt Enable.
*************************************************************************************************
*	Revision History (Description (author, date: yyyy/mm/dd))
*
****************************************************************************************************/
static void _init_TIM3( void )
{
  TIM3->CCMR2 |= (TIM_CCMR2_OC3M_0 | TIM_CCMR2_OC3M_1 |		// (1)
		  TIM_CCMR2_OC3PE				// (2)
  );

  TIM3->CCER |= TIM_CCER_CC3E;					// (3)
  TIM3->ARR = 56;												// (4)
  TIM3->CCR3 = 0;												// (5)
  TIM3->CR1 |= TIM_CR1_URS;					// (6)
  TIM3->DIER |= TIM_DIER_UIE;					// (7)
}

/*			Real-time clock										*/
static void _init_RTC( void )
{

}

/*			Inter-integrated circuit (I2C) interface			*/

/****************************************************************************************************
*   @Brief Description: Init FMPI2C1
*   Function Status: 	PRELIMINARY   (DRAFT , PRELIMINARY, CHECKED, RELEASED)
*
*	************************************************************************************************
*	Function Name:			_init_FMPI2C1()
*	Function Scope:         Local(static)
*	Function Parameters:	void
*	Function Return Type:	void
*	************************************************************************************************
*	@Detailed Description: (Do numbering and tag the number to each part of code)
*	(1) PRESC = 10 --> 4000000MHz
*	(2) SCLDEL = 1 --> 500  ns
*	(3) SDADEL = 1 --> 250  ns
*	(4) SCLH = 1   --> 500  ns
*	(5) SCLL = 4   --> 1250 ns
*	(6) The transfer is not completed after the NBYTES data transfer
*	(7) Setting the no. of bytes to be transfered 1 that each byte there would be a tick to get new value from DMA.
*	(8) Enable Transfer complete interrupt
*	(9) Enable Error flags interrupt
*	(10) Enable NACK interrupt
*	(11) Enable I2C
*	.
*	************************************************************************************************
*	Revision History (Description (author, date: yyyy/mm/dd))
*
****************************************************************************************************/
static void _init_FMPI2C1( void )
{
	//Setting the clock to 400KHz
	FMPI2C1->TIMINGR |= (	(10 << 28) 	|				// (1)
							(1 << 20) 	| 				// (2)
							(1 << 16) 	| 				// (3)
							(1 << 8) 	|				// (4)
							(4 << 0)					// (5)
						);

	FMPI2C1->CR2 |= FMPI2C_CR2_RELOAD;					// (6)
	FMPI2C1->CR2 |= (1 << FMPI2C_CR2_NBYTES_Pos);		// (7)

	//Enable interrupts
	FMPI2C1->CR1 |= (	FMPI2C_CR1_TCIE 	| 			// (8)
						FMPI2C_CR1_ERRIE 	| 			// (9)
						FMPI2C_CR1_NACKIE				// (10)
					);

	FMPI2C1->CR1 |= FMPI2C_CR1_PE;						// (11)
}

/*			Universal asynchronous receiver transmitter			*/

/****************************************************************************************************
*   @Brief Description: Init UART1
*   Function Status: 	PRELIMINARY   (DRAFT , PRELIMINARY, CHECKED, RELEASED)
*
*************************************************************************************************
*	Function Name:			_init_UART1()
*	Function Scope:         Local(static)
*	Function Parameters:	void
*	Function Return Type:	void
*************************************************************************************************
*	@Detailed Description: (Do numbering and tag the number to each part of code)
*	(1) Configuring the baud rate for 250000 => ((176MHz/2) / (16* 22(USART_DIV)))
*	(2) Selecting 2 stop bits
*	(3) Enabling DMA for Transmitter
*	(4) Enabling the Transmitter
*	(1) Creating semaphore for UART1
*************************************************************************************************
*	Revision History (Description (author, date: yyyy/mm/dd))
*
****************************************************************************************************/
static void _init_UART1( void )
{
  USART1->BRR |= (0X16 << 4);				// (1)
  USART1->CR2 |= USART_CR2_STOP_1;			// (2)
  USART1->CR3 |= USART_CR3_DMAT;			// (3)
  USART1->CR1 |= USART_CR1_TE;				// (4)
  USART1BinarySemaphore = xSemaphoreCreateBinary();	// (1)
  //xSemaphoreGive(USART1BinarySemaphore);		//once the binary semaphore is created with vSemaphore.. we should give the semaphore first.
}

/*			Inter-IC sound										*/

/****************************************************************************************************
*   @Brief Description: Init I2S peripheral
*   Function Status: 	DRAFT   (DRAFT , PRELIMINARY, CHECKED, RELEASED)
*
*	************************************************************************************************
*	Function Name:			_init_I2S()
*	Function Scope:         Local(static)
*	Function Parameters:	void
*	Function Return Type:	static void
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
static void _init_I2S( void )
{
	//audio freq. sample =
	//Master mode
	//Data format = 16-bit right justified
	//Packet frame = ?
	//Clock polarity = ?
}

/*			SPDIF receiver interface							*/

/****************************************************************************************************
*   @Brief Description: Init SPDIF
*   Function Status: 	DRAFT   (DRAFT , PRELIMINARY, CHECKED, RELEASED)
*
*	************************************************************************************************
*	Function Name:			_init_SPDIF()
*	Function Scope:         Local(static)
*	Function Parameters:	void
*	Function Return Type:	void
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
static void _init_SPDIF( void )
{
	//we need DMA
	/*
	 * 	Connect the SPDIFRX_IN input to an external interrupt event block in order to detect
		transitions of SPDIFRX_IN line. When activity is detected, then SPDIFRXEN can be
		set to 01 or 11.
	 */
}

/************************************     END OF THE FILE      *************************************/
