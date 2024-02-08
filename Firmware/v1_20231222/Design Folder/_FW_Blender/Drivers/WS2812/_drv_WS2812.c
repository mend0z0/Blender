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
SemaphoreHandle_t TIM3BinarySemaphore;

/****************************************************************************************************
****************************   CONST VARIABLES DECLARATION    ***************************************
*****************************************************************************************************/

/****************************************************************************************************
***********************     STATIC/LOCAL FUNCTIONS DECLARATION      *********************************
*****************************************************************************************************/
uint32_t WS2812ColorConverter( uint8_t red, uint8_t green, uint8_t blue);

/****************************************************************************************************
****************************         GLOBAL FUNTIONS         ****************************************
*****************************************************************************************************/
void _init_WS2812( void )
{
	const TickType_t xDelay50ms = pdMS_TO_TICKS( 50UL );
	uint8_t cnt = 0;
	struct {
			uint32_t pixelColor[WS2812_TEST_PIXEL_NO];
			uint8_t pixelColorRed[WS2812_TEST_PIXEL_NO];
			uint8_t pixelColorGreen[WS2812_TEST_PIXEL_NO];
			uint8_t pixelColorBlue[WS2812_TEST_PIXEL_NO];

	}ws2812;

	for(cnt = WS2812_TEST_PIXEL_NO; cnt > 0; --cnt){
		ws2812.pixelColorRed[(cnt - 1)] = 1;
		ws2812.pixelColorGreen[(cnt - 1)] = 1;
		ws2812.pixelColorBlue[(cnt - 1)] = 1;
	}

	//Turning on the G B R individually to make sure all the colors are in working condition.
	while(1){
		//shift the colors to the left
		ws2812.pixelColorRed[(cnt - 1)] = (ws2812.pixelColorRed[(cnt - 1)] << 1);
		ws2812.pixelColorGreen[(cnt - 1)] = (ws2812.pixelColorGreen[(cnt - 1)] << 1);
		ws2812.pixelColorBlue[(cnt - 1)] = (ws2812.pixelColorBlue[(cnt - 1)] << 1);
		cnt--;
		//reset everything
		if(cnt == 0){
			for(cnt = WS2812_TEST_PIXEL_NO; cnt > 0; --cnt){
				ws2812.pixelColorRed[(cnt - 1)] = 1;
				ws2812.pixelColorGreen[(cnt - 1)] = 1;
				ws2812.pixelColorBlue[(cnt - 1)] = 1;
			}
			cnt = WS2812_TEST_PIXEL_NO;
		}
		//we'll just pick one color and we'll play with that for each pixel.
		ws2812.pixelColor[0] = WS2812ColorConverter( 255, ws2812.pixelColorGreen[0], 0);
		ws2812.pixelColor[1] = WS2812ColorConverter( 0, 255, ws2812.pixelColorBlue[1]);
		ws2812.pixelColor[2] = WS2812ColorConverter( 0, ws2812.pixelColorGreen[2], 255);
		ws2812.pixelColor[3] = WS2812ColorConverter( 255, 102, ws2812.pixelColorBlue[3]);
		ws2812.pixelColor[4] = WS2812ColorConverter( 255, ws2812.pixelColorGreen[4], 102);
		ws2812.pixelColor[5] = WS2812ColorConverter( ws2812.pixelColorRed[5], 255, 102);
		ws2812.pixelColor[6] = WS2812ColorConverter( 178, 255, ws2812.pixelColorBlue[6]);
		//updating the pixels
		WS2812UpdatePixels( &ws2812.pixelColor[0], 7);
		vTaskDelay( xDelay50ms );					//a delay of 500ms
	}
}

int8_t WS2812UpdatePixels( uint32_t *colors/*G R B*/, uint32_t numOfPixels)
{
	uint32_t ledIndexCnt = 0;
	uint8_t colorBitCnt = 0;
	TickType_t semaphoreWatiTime = pdMS_TO_TICKS(10);	// 10msec delay for semaphore to get created, if it didn't happen immediately!
	__IO uint8_t colorBits[WS2812_COLOR_BITS];
	__IO uint32_t tempColor = 0;

	TIM3BinarySemaphore = xSemaphoreCreateBinary();	// Creating semaphore for TIM3

	//check if I can make a semaphore...
	if(TIM3BinarySemaphore == NULL){
		return pdFALSE;
	}

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
			// At this point, we'll wait until we give the semaphore back from ISR
			xSemaphoreTake( TIM3BinarySemaphore, 0);
		}
	}

	TIM3Disable();

	//put a delay of 50us second. from FreeRTOS library

	return pdTRUE; //as it's completed the update of string.
}


/****************************************************************************************************
****************************         STATIC FUNTIONS         ****************************************
*****************************************************************************************************/
uint32_t WS2812ColorConverter( uint8_t red, uint8_t green, uint8_t blue)
{
	return ((green <<  16) | (red << 8) | blue);
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
