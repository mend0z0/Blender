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
#include "_drv_ws2812.h"

/****************************************************************************************************
****************************   GLOB. VARIABLES DECLARATION    ***************************************
*****************************************************************************************************/

/****************************************************************************************************
****************************   CONST VARIABLES DECLARATION    ***************************************
*****************************************************************************************************/

/****************************************************************************************************
***********************     STATIC/LOCAL FUNCTIONS DECLARATION      *********************************
*****************************************************************************************************/

/****************************************************************************************************
****************************         GLOBAL FUNTIONS         ****************************************
*****************************************************************************************************/
void _init_WS2812( void )
{
	uint32_t testColor[3][7] = {{0X00FF0000, 0X00FF0000, 0X00FF0000, 0X00FF0000, 0X00FF0000, 0X00FF0000, 0X00FF0000},
								{0X0000FF00, 0X0000FF00, 0X0000FF00, 0X0000FF00, 0X0000FF00, 0X0000FF00, 0X0000FF00},
								{0X000000FF, 0X000000FF, 0X000000FF, 0X000000FF, 0X000000FF, 0X000000FF, 0X000000FF}
								};
	//Turning on the G B R individually to make sure all the colors are in working condition.
	for(uint8_t cnt = 0; cnt < 3; ++cnt){
		WS2812UpdatePixels( &testColor[cnt], 7);
		//put a delay of 500ms from FreeRTOS library
	}
}

int8_t WS2812UpdatePixels( uint32_t *colors/*G R B*/, uint32_t numOfPixels)
{
	uint32_t ledIndexCnt = 0;
	uint8_t colorBitCnt = 0;
	__IO uint8_t colorBits[WS2812_COLOR_BITS];
	__IO uint32_t tempColor = 0;

	//check if I can make a semaphore...

	TIM3Enable();

	for(ledIndexCnt = 0; ledIndexCnt < numOfPixels; ++ledIndexCnt)
	{
		tempColor = *(colors + ledIndexCnt);
		for(colorBitCnt = 0; colorBitCnt < WS2812_COLOR_BITS; ++colorBitCnt)
		{
			if((tempColor & (1 << 23)) == (1 << 23))
			{
				TIM3UpdateCCR3( WS2812_T1H );
			}
			else
			{
				TIM3UpdateCCR3( WS2812_T0H );
			}
			//take a semaphore()
		}
	}

	TIM3Disable();

	//put a delay of 50us second. from FreeRTOS library

	return true; //as it's completed the update of string.
}


/****************************************************************************************************
****************************         STATIC FUNTIONS         ****************************************
*****************************************************************************************************/

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
