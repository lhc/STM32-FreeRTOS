/**
 * @file    app_shell.c
 * @author  Jorge Guzman
 * @date    Jul 15, 2017
 * @version 0.1.0.0 (beta)
 * @brief   Lib que configura e usa a callback do micro-shell
 */

//==============================================================================
// INCLUDE FILES
//==============================================================================

#include "app_shell.h"
#include "sensores.h"
#include "setup_hw.h"

#include "leds/leds.h"
#include "micro-shell/micro-shell.h"
#include "freertos_utils/freertos_utils.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#if defined(USE_SYSVIEW)
#include "SEGGER_SYSVIEW.h"
#endif

//==============================================================================
// PRIVATE DEFINITIONS
//==============================================================================

//==============================================================================
// PRIVATE TYPEDEFS
//==============================================================================

//==============================================================================
// PRIVATE VARIABLES
//==============================================================================

Sensors_t StrSensor;
TaskHandle_t xHandleTaskCPU = NULL;
BaseType_t xReturned;

//==============================================================================
// PRIVATE FUNCTIONS
//==============================================================================

static HAL_StatusTypeDef Help_Commads(void);
static HAL_StatusTypeDef Sensors_CommandLine(uint16_t argc, uint8_t **argv);
static HAL_StatusTypeDef Leds_CommandLine(uint16_t argc, uint8_t **argv);

//==============================================================================
// SOURCE CODE
//==============================================================================

void taskUseCPU(void *pvParameters)
{
	unsigned int i, j;

	for (;;) {

		for (i = 0, j = 0; i < 10000; i++){
			j *= i + 12.34;
		}

		vTaskDelay(100/portTICK_RATE_MS);
	}
}

static HAL_StatusTypeDef Help_Commads(void)
{
	SHELL_PRINTF("Supported commands:\r\n");
	SHELL_PRINTF("> rtos");
	SHELL_PRINTF("> get <v1>");
	SHELL_PRINTF("\t temperature");
	SHELL_PRINTF("\t humidity");
	SHELL_PRINTF("\t pressure");
	SHELL_PRINTF("\t gyro");
	SHELL_PRINTF("\t magneto");
	SHELL_PRINTF("\t accelero");
	SHELL_PRINTF("> led <v1>");
	SHELL_PRINTF("\t on");
	SHELL_PRINTF("\t off");
	SHELL_PRINTF("\t blink");
	SHELL_PRINTF("\t heartbeat");
	SHELL_PRINTF("> consume");
	SHELL_PRINTF("\t create");
	SHELL_PRINTF("\t delete");

	return HAL_OK;
}

static HAL_StatusTypeDef Sensors_CommandLine(uint16_t argc, uint8_t **argv)
{
	HAL_StatusTypeDef err = HAL_OK;

	if (strcmp((const char *) "temperature", (const char *) argv[0]) == 0)
	{
		Temperature_Test(&StrSensor);
	}
	else if (strcmp((const char *) "humidity", (const char *) argv[0]) == 0)
	{
		Humidity_Test(&StrSensor);
	}
	else if (strcmp((const char *) "pressure", (const char *) argv[0]) == 0)
	{
		Pressure_Test(&StrSensor);
	}
	else if (strcmp((const char *) "gyro", (const char *) argv[0]) == 0)
	{
		Gyro_Test(&StrSensor);
	}
	else if (strcmp((const char *) "magneto", (const char *) argv[0]) == 0)
	{
		Magneto_Test(&StrSensor);
	}
	else if (strcmp((const char *) "accelero", (const char *) argv[0]) == 0)
	{
		Accelero_Test(&StrSensor);
	}
	else
	{
		err = HAL_ERROR;
	}

	return err;
}

static HAL_StatusTypeDef Leds_CommandLine(uint16_t argc, uint8_t **argv)
{
	HAL_StatusTypeDef err = HAL_OK;

	if (strcmp((const char *) "on", (const char *) argv[0]) == 0)
	{
		Leds_Set(N_LED1, LED_ON);
	}
	else if (strcmp((const char *) "off", (const char *) argv[0]) == 0)
	{
		Leds_Set(N_LED1, LED_OFF);
	}
	else if (strcmp((const char *) "blink", (const char *) argv[0]) == 0)
	{
		Leds_Set(N_LED1, LED_BLINK_SLOW);
	}
	else if (strcmp((const char *) "heartbeat", (const char *) argv[0]) == 0)
	{
		Leds_Set(N_LED1, LED_BLINK_HEARTBEAT);
	}
	else
	{
		err = HAL_ERROR;
	}

	return err;
}

void Shell_Callback(uint8_t *cmd, uint16_t argc, uint8_t **argv)
{
	HAL_StatusTypeDef resp = HAL_ERROR;

	if (strcmp((const char *) "rtos", (const char *) cmd) == 0)
	{
		RTOS_DBG_ShowStatus();
		resp = HAL_OK;
	}
	if (strcmp((const char *) "help", (const char *) cmd) == 0)
	{
		resp = Help_Commads();
	}
	else if (strcmp((const char *) "get", (const char *) cmd) == 0)
	{
		resp = Sensors_CommandLine(argc, argv);
	}
	else if (strcmp((const char *) "led", (const char *) cmd) == 0)
	{
		resp = Leds_CommandLine(argc, argv);
	}
	else if((strcmp((const char *) "consume", (const char *) cmd) == 0))
	{
		if (strcmp((const char *) "create", (const char *) argv[0]) == 0)
		{
			if(xHandleTaskCPU == NULL)
			{
				//create task to consume some cpu
				xReturned = xTaskCreate(taskUseCPU, (char * )"TaskCPU", configMINIMAL_STACK_SIZE, NULL, 5, &xHandleTaskCPU);
				DBG_ASSERT_PARAM(xReturned);
			}
		}
		else if (strcmp((const char *) "delete", (const char *) argv[0]) == 0)
		{
			if( xHandleTaskCPU != NULL )
			{
				//delete task to consume some cpu
				vTaskDelete( xHandleTaskCPU );
			}
		}
	}
	else
	{
#if defined(USE_SYSVIEW)
		SEGGER_SYSVIEW_ErrorfTarget("(X) CMD Shell: %s", cmd);
#endif
	}


#if defined(USE_SYSVIEW)
	if(resp == HAL_OK)
	{
		SEGGER_SYSVIEW_PrintfTarget("CMD Shell: %s", cmd);
	}
#endif

}
