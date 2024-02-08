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
#include "_drv_DMX.h"

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
void _init_DMX( void )
{
	uint8_t cnt;
	const TickType_t xDelay100ms = pdMS_TO_TICKS( 100UL );

	struct{
			uint8_t colorRed;
			uint8_t colorGreen;
			uint8_t colorBlue;
	}dmx_head[DMX_HEAD_MAX];

	for(cnt = DMX_HEAD_MAX; cnt > 0; --cnt)
	{
		dmx_head[(cnt - 1)].colorRed = 0x01;
		dmx_head[(cnt - 1)].colorGreen = 0x01;
		dmx_head[(cnt - 1)].colorBlue = 0x01;
	}

	while(1)
	{
		//shifting the light values to the left
		dmx_head[(cnt - 1)].colorRed = (dmx_head[(cnt - 1)].colorRed << 1) | 0x01;
		dmx_head[(cnt - 1)].colorGreen = (dmx_head[(cnt - 1)].colorGreen << 1) | 0x02;
		dmx_head[(cnt - 1)].colorBlue = (dmx_head[(cnt - 1)].colorBlue << 1) | 0x04;
		cnt--;
		//reseting the counter.
		if(cnt == 0)
		{
			cnt = DMX_HEAD_MAX;
		}
		//Updating the colors for all the heads.
		DMXHeadUpdate( &dmx_head[0].colorRed, (DMX_HEAD_MAX *3));
		//100 milliseconds delay
		vTaskDelay(xDelay100ms);
	}
}

int8_t DMXHeadUpdate( uint8_t *colors /* R G B */, uint32_t numOfHead /*it should be (numOfHead x 3)*/)
{
	return UART1DataTx( *colors, numOfHead);
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
