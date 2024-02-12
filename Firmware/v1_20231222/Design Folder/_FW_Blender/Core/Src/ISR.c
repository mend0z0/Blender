/****************************************************************************************************
*
*   Project Name:       Blender
*   @Brief Description: Blending audio spec with light spectrum
*   File Status:	    DRAFT   (DRAFT , PRELIMINARY, CHECKED, RELEASED)
*
*	File Name:	xxxx.c
*	Version:	01
*	Revision:	01
*	Date:		2023/01/dd
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
#include "isr.h"
/****************************************************************************************************
****************************   CONST VARIABLES DECLARATION    ***************************************
*****************************************************************************************************/
const uint32_t LED_IND_DELAY = 100;				// This is 100 milliseconds
const uint32_t DMA1_Stream4_IRQ_PRIORITY = 10;
const uint32_t DMA2_Stream6_IRQ_PRIORITY = 11;
const uint32_t TIM3_IRQ_PRIORITY = 12;
const uint32_t FMPI2C1_EV_IRQ_PRIORITY = 10;
const uint32_t USART1_IRQ_PRIORITY = 13;

/****************************************************************************************************
****************************   GLOB. VARIABLES DECLARATION    ***************************************
*****************************************************************************************************/
extern SemaphoreHandle_t FMPI2CBinarySemaphore;
extern SemaphoreHandle_t USART1BinarySemaphore;
extern SemaphoreHandle_t TIM3BinarySemaphore;

uint32_t ledIndCnt = LED_IND_DELAY;			// This is a counter to toggle the LED once it's it's underflowed.

/****************************************************************************************************
***********************     STATIC/LOCAL FUNCTIONS DECLARATION      *********************************
*****************************************************************************************************/

/****************************************************************************************************
****************************         GLOBAL FUNTIONS         ****************************************
*****************************************************************************************************/
void _init_ISR( void )
{

	NVIC_SetPriority(DMA1_Stream4_IRQn	, DMA1_Stream4_IRQ_PRIORITY);
	NVIC_SetPriority(DMA2_Stream6_IRQn	, DMA2_Stream6_IRQ_PRIORITY);
	NVIC_SetPriority(TIM3_IRQn			, TIM3_IRQ_PRIORITY);
	NVIC_SetPriority(FMPI2C1_EV_IRQn	, FMPI2C1_EV_IRQ_PRIORITY);
	NVIC_SetPriority(USART1_IRQn		, USART1_IRQ_PRIORITY);

	NVIC_EnableIRQ(DMA1_Stream4_IRQn);
	NVIC_EnableIRQ(DMA2_Stream6_IRQn);
	NVIC_EnableIRQ(TIM3_IRQn);
	NVIC_EnableIRQ(FMPI2C1_EV_IRQn);
	NVIC_EnableIRQ(USART1_IRQn);
	NVIC_EnableIRQ(SysTick_IRQn);
}

/****************************************************************************************************
****************************         STATIC FUNTIONS         ****************************************
*****************************************************************************************************/


/*			SysTick Timer	 									*/
void SysTick_Handler( void )
{
	//Blinking the Indicator LED
	ledIndCnt--;
	if(ledIndCnt == 0)
	{
		ledIndCnt = LED_IND_DELAY;
		TGL_PIN( LED_IND_PORT, LED_IND_PIN);
	}
}

/*			Power controller 									*/


/*			Reset and clock control 							*/


/*			General-purpose I/Os								*/


/*			Direct memory access controller						*/
void DMA1_Stream0_IRQHandler( void )
{

}

void DMA1_Stream1_IRQHandler( void )
{

}

void DMA1_Stream2_IRQHandler( void )
{

}

void DMA1_Stream3_IRQHandler( void )
{

}

void DMA1_Stream4_IRQHandler( void )
{
	DMA1_Stream5->CR &= ~DMA_SxCR_TCIE;		// Disable Transfer complete interrupt.
	FMPI2C1->CR2 |= FMPI2C_CR2_STOP;		//Upon completion sending the stop for releasing the line.
	//this will give back the semaphore so we'll back to continue FMPI2CDataTx
	xSemaphoreGiveFromISR( FMPI2CBinarySemaphore, pdFALSE);
}

void DMA1_Stream5_IRQHandler( void )
{

}

void DMA1_Stream6_IRQHandler( void )
{

}

void DMA2_Stream0_IRQHandler( void )
{

}

void DMA2_Stream1_IRQHandler( void )
{

}

void DMA2_Stream2_IRQHandler( void )
{

}

void DMA2_Stream3_IRQHandler( void )
{

}

void DMA2_Stream4_IRQHandler( void )
{

}

void DMA2_Stream5_IRQHandler( void )
{

}

void DMA2_Stream6_IRQHandler( void )
{
	DMA2->HIFCR |= DMA_HIFCR_CTCIF7;	// Clearing the flag by writing a one.
	DMA2_Stream7->CR &= ~DMA_SxCR_TCIE;	// Disable Transfer complete interrupt.
}

/*			Analog-to-digital converter	1						*/


/*			Analog-to-digital converter	2						*/


/*			Analog-to-digital converter	3						*/


/*			Digital-to-analog converter							*/


/*			Advanced-control timers (TIM1)						*/


/*			General-purpose timers (TIMER2)						*/


/*			General-purpose timers (TIMER3)						*/
void TIM3_IRQHandler( void ){
	TIM3->SR &= ~TIM_SR_UIF;
	xSemaphoreGiveFromISR( TIM3BinarySemaphore, pdFALSE);
}

/*			Real-time clock										*/


/*			Inter-integrated circuit (I2C) interface			*/
void FMPI2C1_EV_IRQHandler( void )
{
	if((FMPI2C1->ISR & FMPI2C_ISR_NACKF) == FMPI2C_ISR_NACKF)
	{
		//Disable the NACK interrupt for servicing it outside of ISR.
		FMPI2C1->CR1 &= ~FMPI2C_CR1_NACKIE;
		//this will give back the semaphore so we'll back to continue FMPI2CDataTx
		xSemaphoreGiveFromISR( FMPI2CBinarySemaphore, pdFALSE);
	}
	else if((FMPI2C1->ISR & FMPI2C_ISR_TXIS) == FMPI2C_ISR_TXIS)
	{
		//Enabling the DMA for transferring the data via DMA
		FMPI2C1->CR1 |= FMPI2C_CR1_TXDMAEN;
		//No more needed after confirming the slave address. So we disable TXIE.
		FMPI2C1->CR1 &= ~FMPI2C_CR1_TXIE;
	}

}

void FMPI2C1_ER_IRQHandler( void )
{

}

/*			Universal asynchronous receiver transmitter			*/
void USART1_IRQHandler( void )
{
	USART1->CR1 &= ~USART_CR1_TCIE;							//disabling the USART interrupt
	xSemaphoreGiveFromISR( USART1BinarySemaphore, pdFALSE);	//Giving back the semaphore
}

/*			Inter-IC sound										*/


/*			SPDIF receiver interface							*/



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
