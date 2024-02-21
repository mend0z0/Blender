/****************************************************************************************************
*
*   Project Name:       Blender
*   @Brief Description: Blending audio spec with light spectrum
*   File Status:	    PRELIMINARY   (DRAFT , PRELIMINARY, CHECKED, RELEASED)
*
*	File Name:	_drv_DMX.c
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
#include "_drv_DMX.h"

/****************************************************************************************************
****************************   GLOB. VARIABLES DECLARATION    ***************************************
*****************************************************************************************************/

/****************************************************************************************************
****************************   CONST VARIABLES DECLARATION    ***************************************
*****************************************************************************************************/

const TickType_t xDelay100ms = pdMS_TO_TICKS( 100UL );	// a 100 milliseconds delay

struct dmx_colors{										// A structure of RGB (8/8/8) colors for DMX receivers.
		uint8_t colorRed;
		uint8_t colorGreen;
		uint8_t colorBlue;
};

/****************************************************************************************************
***********************     STATIC/LOCAL FUNCTIONS DECLARATION      *********************************
*****************************************************************************************************/

/****************************************************************************************************
****************************         GLOBAL FUNTIONS         ****************************************
*****************************************************************************************************/

/****************************************************************************************************
*   @Brief Description:	DMX Standby mode, simply just rolling different color on line
*   Function Status: 	PRILIMINARY   (DRAFT , PRILIMINARY, CHECKED, RELEASED)
*
*	************************************************************************************************
*	Function Name:			_init_DMX()
*	Function Scope:			Global
*	Function Parameters:	void *pvParameters
*	Function Return Type:	void
*	************************************************************************************************
*	@Detailed Description: (Do numbering and tag the number to each part of code)
*	(1) Initializing the DMX head variables (RGB) with reset value.
*	(2)	Creating an infinite loop that the functions need to run in it.
*	(3) Shifting 1 bit to the left (simply changing the value) of the color R
*	(4) Shifting 1 bit to the left (simply changing the value) of the color G
*	(5) Shifting 1 bit to the left (simply changing the value) of the color B
*	(6) Decrementing the head counter by 1
*	(7) Check if we the counter reached zero (it's an unsigned variable and shouldn't go negative or overflow)
*	(8) If we've reached 0, we'll feed the counter with the maximum number of DMX head we have
*	(9) Feeding the DMXHeadUpdate with new value to be updated for each head.
*	(10) Creating a 100 milliseconds delay and starting over from line 3
*	.
*	************************************************************************************************
*	Revision History (Description, author, date: yyyy/mm/dd)
*
****************************************************************************************************/
void _init_DMX( void *pvParameters)
{
	uint8_t cnt;
	struct dmx_colors dmx_head[DMX_MAX_HEAD];	// Creating 7 different DMX receiver

	//	(1)
	for(cnt = DMX_MAX_HEAD; cnt > 0; --cnt)
	{
		dmx_head[(cnt - 1)].colorRed = DMX_RESET_VALUE_R;
		dmx_head[(cnt - 1)].colorGreen = DMX_RESET_VALUE_G;
		dmx_head[(cnt - 1)].colorBlue = DMX_RESET_VALUE_B;
	}

	while(1)																							// (2)
	{
		dmx_head[(cnt - 1)].colorRed = (dmx_head[(cnt - 1)].colorRed << 1) | DMX_CONSTANT_VALUE_R;		// (3)
		dmx_head[(cnt - 1)].colorGreen = (dmx_head[(cnt - 1)].colorGreen << 1) | DMX_CONSTANT_VALUE_G;	// (4)
		dmx_head[(cnt - 1)].colorBlue = (dmx_head[(cnt - 1)].colorBlue << 1) | DMX_CONSTANT_VALUE_B;	// (5)

		cnt--;																							// (6)

		if(cnt == 0)																					// (7)
		{
			cnt = DMX_MAX_HEAD;																			// (8)
		}

		DMXHeadUpdate( &dmx_head[0].colorRed, (DMX_MAX_HEAD *3));										// (9)

		vTaskDelay(xDelay100ms);																		// (10)
	}
}

/****************************************************************************************************
*   @Brief Description:	Feeding the UART Buffer to transfer the values on DMX line.
*   Function Status: 	PRILIMINARY   (DRAFT , PRILIMINARY, CHECKED, RELEASED)
*
*	************************************************************************************************
*	Function Name:			DMXHeadUpdate()
*	Function Scope:			Global
*	Function Parameters:	uint8_t *colors  (R G B), uint32_t numOfHead (it should be (numOfHead x 3))
*	Function Return Type:	void
*	************************************************************************************************
*	@Detailed Description: (Do numbering and tag the number to each part of code)
*	(1) Referencing the colors variable to UART Buffer.
*	.
*	************************************************************************************************
*	Revision History (Description, author, date: yyyy/mm/dd)
*
****************************************************************************************************/
int8_t DMXHeadUpdate( uint8_t *colors, uint32_t numOfHead)
{
	return (int8_t)UART1DataTx( *colors, numOfHead);	// (1)
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
