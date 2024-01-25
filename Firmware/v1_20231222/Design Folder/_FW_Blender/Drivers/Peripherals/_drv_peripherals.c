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

/*			Analog-to-digital converter							*/
static void _init_ADC( void );

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
	_init_DMA();
	_init_ADC();
	_init_TIM1();
	_init_RTC();
	_init_GPIO();
	_init_FMPI2C1();
	_init_TIM2();
	_init_TIM3();
	_init_UART1();
	_init_I2S();
	_init_SPDIF();
}

/*			General-purpose I/Os								*/

/*			Direct memory access controller						*/

/*			Analog-to-digital converter							*/

/*			Advanced-control timers (TIM1)						*/

/*			General-purpose timers (TIMER3)						*/

/*			Real-time clock										*/

/*			Inter-integrated circuit (I2C) interface			*/

/*			Universal asynchronous receiver transmitter			*/

/*			Inter-IC sound										*/

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
					);
	//Enable DMA
	//RCC->AHB1ENR |= RCC_AHB1ENR_DAM1;
	//RCC->AHB1ENR |= RCC_AHB1ENR_DAM2;
	//Enable ADC (Each ADC for one channel of RGB VGA)
	//RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
	//RCC->APB2ENR |= RCC_APB2ENR_ADC2EN;
	//RCC->APB2ENR |= RCC_APB2ENR_ADC3EN;
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

}

/*			Direct memory access controller						*/
static void _init_DMA( void )
{

}

/*			Analog-to-digital converter							*/
static void _init_ADC( void )
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

}

/*			Real-time clock										*/
static void _init_RTC( void )
{

}

/*			Inter-integrated circuit (I2C) interface			*/
static void _init_FMPI2C1( void )
{

}

/*			Universal asynchronous receiver transmitter			*/
static void _init_UART1( void )
{

}

/*			Inter-IC sound										*/
static void _init_I2S( void )
{

}

/*			SPDIF receiver interface							*/
static void _init_SPDIF( void )
{

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
