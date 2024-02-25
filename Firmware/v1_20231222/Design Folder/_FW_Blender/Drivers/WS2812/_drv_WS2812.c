/****************************************************************************************************
*
*   Project Name:       Blender
*   @Brief Description: Blending audio spec with light spectrum
*   File Status:	    PRELIMINARY   (DRAFT , PRELIMINARY, CHECKED, RELEASED)
*
*	File Name:	_drv_WS2812.c
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
#include "_drv_ws2812.h"


/****************************************************************************************************
****************************   CONST VARIABLES DECLARATION    ***************************************
*****************************************************************************************************/
static const TickType_t xDelay500ms = pdMS_TO_TICKS( 500UL );	// Defining a 100 milliseconds delay

struct ws2812_color{											// Creating a structure of RGB (8/8/8) for WS2812 pixels.
 			uint8_t red;
 			uint8_t green;
 			uint8_t blue;
};

/****************************************************************************************************
****************************   GLOB. VARIABLES DECLARATION    ***************************************
*****************************************************************************************************/

__IO uint8_t ws2812ColorBits[WS2812_NO_COLOR_BITS];
__IO uint8_t ws2812ColorBitIndexCnt = 0;
struct ws2812_color ws2812_pixel[WS2812_MAX_PIXEL_ONBOARD];			// Creating 7 pixels of ws2812.

/****************************************************************************************************
***********************     STATIC/LOCAL FUNCTIONS DECLARATION      *********************************
*****************************************************************************************************/


/****************************************************************************************************
****************************         GLOBAL FUNTIONS         ****************************************
*****************************************************************************************************/

/****************************************************************************************************
*   @Brief Description:	Start up and standby routine for the onboard WS2812 pixels
*   Function Status: 	PRILIMINARY   (DRAFT , PRILIMINARY, CHECKED, RELEASED)
*
*************************************************************************************************
*	Function Name:		_init_WS2812()
*	Function Scope:		Global
*	Function Parameters:	void *pvParameters
*	Function Return Type:	void
*************************************************************************************************
*	@Detailed Description: (Do numbering and tag the number to each part of code)
*	(1) Initializing the structure variables with defined reset values.
*	(2) Creating an infinite loop to run the color patterns and the task executes.
*	(3) Shift the color Red to the left and add a constant to the value.
*	(4) Shift the color Green to the left and add a constant to the value.
*	(5) Shift the color Blue to the left and add a constant to the value.
*	(6) Uploading the pixelColor with the new colors value.
*	(7) Adding the pixel index counter by 1
*	(8) Check if we reached to the maximum pixel index.
*	(9) Reset the pixel index counter to 0.
*	(10) Updating the pixels
*	(11) A delay of 100ms
*************************************************************************************************
*	Revision History (Description, author, date: yyyy/mm/dd)
*
****************************************************************************************************/
void _init_WS2812( void *pvParameters )
{
  uint8_t cnt = 0;								// A counter for counting the ws2812 pixel indexes.
  __IO uint32_t ws2812Pixel[WS2812_MAX_PIXEL_ONBOARD];

  // (1)
  for(cnt = 0; cnt < WS2812_MAX_PIXEL_ONBOARD; ++cnt){
      ws2812_pixel[cnt].red = WS2812_RESET_VALUE_RED;
      ws2812_pixel[cnt].green = WS2812_RESET_VALUE_GREEN;
      ws2812_pixel[cnt].blue = WS2812_RESET_VALUE_BLUE;

      ws2812Pixel[cnt] = RGB_TO_GRB(	ws2812_pixel[cnt].red,
					ws2812_pixel[cnt].green,
					ws2812_pixel[cnt].blue
      );
  }

  while(1)											// (2)
    {

      ws2812_pixel[cnt].red = ((ws2812_pixel[cnt].red << 1) | WS2812_CONSTANT_VALUE_RED);	// (3)
      ws2812_pixel[cnt].green = ((ws2812_pixel[cnt].green << 1) | WS2812_CONSTANT_VALUE_GREEN);	// (4)
      ws2812_pixel[cnt].blue = ((ws2812_pixel[cnt].blue << 1) | WS2812_CONSTANT_VALUE_BLUE);	// (5)

      ws2812Pixel[cnt] = RGB_TO_GRB(	ws2812_pixel[cnt].red,					// (6)
					ws2812_pixel[cnt].green,
					ws2812_pixel[cnt].blue
      );

      cnt++;											// (7)

      if(cnt == WS2812_MAX_PIXEL_ONBOARD){							// (8)
	  cnt = 0;										// (9)
      }

      WS2812UpdatePixels( ws2812Pixel, WS2812_MAX_PIXEL_ONBOARD);				// (10)

      vTaskDelay( xDelay500ms );								// (11)
    }
}

/****************************************************************************************************
*   @Brief Description:	Updating the ws2812 pixels with new values.
*   Function Status: 	PRILIMINARY   (DRAFT , PRILIMINARY, CHECKED, RELEASED)
*
*************************************************************************************************
*	Function Name:		WS2812UpdatePixels()
*	Function Scope:		Global
*	Function Parameters:	__IO uint32_t *colors (G R B), uint32_t numOfPixels
*	Function Return Type:	int8_t
*************************************************************************************************
*	@Detailed Description: (Do numbering and tag the number to each part of code)
*	(4) Enabling TIM3 in output compare mode
*	(5) Creating a loop to go through all the ws2812 pixels
*	(6) Uploading the selected ws2812 pixel color to a temporary variable
*	(7) Creating a loop for counting bits of ws2812 pixel (which is 24) and sending out bit values one by one
*	(8) Check if the 23rd bits is one
*	(9) Then updating TIM3 CCR value with WS2812_T1H constant
*	(10) Otherwise the 23rd bit is zero and we'll update TIM3 register with WS2812_T0H value
*	(12) Creating a loop to send out 40 zero bits (It'll create a 50 microseconds delay as of a reset signal)
*	(13) Updating the TIM3 register with zero
*	(14) Waiting to give back the semaphore that we've taken.
*	(15) Disabling TIM3
*	(16) Returning pdTRUE (1) as of successful update of WS2812 pixels.
*************************************************************************************************
*	Revision History (Description, author, date: yyyy/mm/dd)
*
****************************************************************************************************/
int8_t WS2812UpdatePixels( __IO uint32_t *colors, uint32_t numOfPixels)
{
  uint32_t ledIndexCnt = 0;
  uint8_t colorBitIndexCnt = 0;
  bool tim3UpdateStatus = 0;
  __IO uint32_t tempColor = 0;

  TIM3Enable();									// (4)

  for(ledIndexCnt = 0; ledIndexCnt < numOfPixels; ++ledIndexCnt)		// (5)
    {
      tempColor = *(colors + ledIndexCnt);					// (6)

      for(colorBitIndexCnt = 0; colorBitIndexCnt < WS2812_NO_COLOR_BITS; ++colorBitIndexCnt)	// (7)
	{
	  if((tempColor & (1 << 23)) == (1 << 23))				// (8)
	    {
	      ws2812ColorBits[colorBitIndexCnt] = WS2812_T1H;					// (9)
	    }
	  else
	    {
	      ws2812ColorBits[colorBitIndexCnt] = WS2812_T1H;					// (10)
	    }
	  tempColor = (tempColor << 1);
	}
      TIM3UpdateCCR3();
    }
  for(colorBitIndexCnt = 0; colorBitIndexCnt < WS2812_RES; ++colorBitIndexCnt)			// (12)
    {
      for(colorBitIndexCnt = 0; colorBitIndexCnt < WS2812_NO_COLOR_BITS; ++colorBitIndexCnt)	// (7)
      	{
	  ws2812ColorBits[colorBitIndexCnt] = 0;
      	}
      TIM3UpdateCCR3();						// (13)
    }

  TIM3Disable();								// (15)

  return pdTRUE; 								// (16)
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
