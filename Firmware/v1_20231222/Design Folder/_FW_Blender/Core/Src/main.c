/****************************************************************************************************
*
*   	Project Name:       Blender
*   	@Brief Description: Blending audio spec with light spectrum
*   	File Status:	    DRAFT   (DRAFT , PRELIMINARY, CHECKED, RELEASED)
*
*	File Name:	main.c
*	Version:	01
*	Revision:	01
*	Date:		2024/02/20
*	License:	Open-source
********************************* Project Description *******************************************
*		@Detailed Description: The Blender project's purpose is to extract digitalized sound characteristics
*   	like amplitude and frequency, which are transferring through the S/PDIF protocol, and display the amplitude
*   	of different frequencies by the dominant color of the input VGA received signal. So the base color will be
*   	determined by VGA input, and a spectrum of that color will be displayed over/on the DMX/WS2812 chip. Also,
*   	the specification of the sound will be displayed on an OLED, and the sound will be played by a chip that has
*   	an integrated DAC in it. The data to DAC will be transferred via the I2S protocol.
*
********************************* File Description *******************************************
*	@Detailed Description:
*
*
**********************************  Hardware Info  **********************************************
*  	Name:       _HW_Blender
*   	Version:    v1.0
**********************************  Processor Info **********************************************
*	Processor:          STM32F446RCT6 32-Bits
*	Clock Frequency:    180.000	MHz
*	RAM/SRAM Size:      128.00	KBytes
*	Flash Size:         256.00	KBytes
************************************  Case Style  ***********************************************
*	Local Variables = camelCase			Global Variables = g_camelCase
*	Headers = SCREAMIN_SNAKE_CASE
*   	Macros = SCREAMING_SNAKE_CASE
*   	Const Variables = SCREAMING_SNAKE_CASE
*	Functions = PascalCase				Initialize Functions = _lower_case
*	Struct = lower_case					Struct.members = camelCase
*	Enum = lower_case					Enum Members = SNAKE_CASE
*	Pointer Variables = p_camelCase
************************************ Contact Info ***********************************************
*	Author:	Siavash Taher Parvar
*	Github: github.com/Mend0z0
****************************************************************************************************/

/****************************************************************************************************
****************************       HEADERS DECLARATION       ****************************************
*****************************************************************************************************/
#include "main.h"

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

/****************************************************************************************************
****************************         STATIC FUNTIONS         ****************************************
*****************************************************************************************************/
int main(void)
{
  /* 	----------------------   		MCU Configuration				----------------------------
	----------------------   Initialize all configured peripherals  ----------------------------*/
  _init_Peripherals();
  _init_ISR();
  /* ----------------------   	  Modules Configuration 			----------------------------*/
  SET_BIT( EXT_LDO_EN_PORT, EXT_LDO_EN_PIN);	//Enable the LDO power to turn the modules on
  _init_PAM8003();
  while(1);
  //Each module init will be a task...

  xTaskCreate( 	_init_DMX, 	// Pointer to the function that implements the task.
		"DMX", 		// Text name for the task.
		1000,		// Stack depth.
		NULL,		// This example does not use the task parameter.
		2,		// This task will run at priority 2.
		NULL		// This example does not use the task handle.
  );

  xTaskCreate(	_init_WS2812, 	// Pointer to the function that implements the task.
		"WS2812", 	// Text name for the task.
		500,		// Stack depth.
		NULL,		// This example does not use the task parameter.
		2,		// This task will run at priority 2.
		NULL		// This example does not use the task handle.
  );

  //xTaskCreate( 	_init_SSD1306, 	// Pointer to the function that implements the task.
  //			"SSD1306", 		// Text name for the task.
  //			5000,		// Stack depth.
  //			NULL,		// This example does not use the task parameter.
  //			4,			// This task will run at priority 2.
  //			NULL		// This example does not use the task handle.
  //		);
  /* Start the created tasks running. */
  vTaskStartScheduler();

  while (1)
    {
      //SET_BIT( LED_IND_PORT, LED_IND_PIN)
    }
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

