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
****************************   GLOB. VARIABLES DECLARATION    ***************************************
*****************************************************************************************************/
extern SemaphoreHandle_t FMPI2CBinarySemaphore;

/****************************************************************************************************
****************************   CONST VARIABLES DECLARATION    ***************************************
*****************************************************************************************************/

/****************************************************************************************************
***********************     STATIC/LOCAL FUNCTIONS DECLARATION      *********************************
*****************************************************************************************************/

/****************************************************************************************************
****************************         GLOBAL FUNTIONS         ****************************************
*****************************************************************************************************/
void _init_ISR( void )
{
	NVIC_SetPriority();
}

/****************************************************************************************************
****************************         STATIC FUNTIONS         ****************************************
*****************************************************************************************************/





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
	//ONCE THE COMPLETE TRANSFER HAS BEEN TRIGGERED I SHOULD TURN OFF THE FMPI2C.
	//Upon completion sending the stop for releasing the line.
	FMPI2C1->CR2 |= FMPI2C_CR2_STOP;
	//this will give back the semaphore so we'll back to continue FMPI2CDataTx
	xSemaphoreGiveFromISR( FMPI2CBinarySemaphore, pdFALSE);
}

void DMA1_Stream5_IRQHandler( void )
{

}

void DMA1_Stream6_IRQHandler( void )
{

}

/*			Analog-to-digital converter	1						*/


/*			Analog-to-digital converter	2						*/


/*			Analog-to-digital converter	3						*/


/*			Digital-to-analog converter							*/


/*			Advanced-control timers (TIM1)						*/


/*			General-purpose timers (TIMER2)						*/


/*			General-purpose timers (TIMER3)						*/


/*			Real-time clock										*/


/*			Inter-integrated circuit (I2C) interface			*/
void FMPI2C1_EV_IRQHandler( void )
{
	if((FMPI2C1->ISR & FMPI2C_ISR_NACKF) == FMPI2C_ISR_NACKF)
	{
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
