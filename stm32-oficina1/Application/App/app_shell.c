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

//==============================================================================
// PRIVATE FUNCTIONS
//==============================================================================

static HAL_StatusTypeDef Help_Commads(void);
static HAL_StatusTypeDef Sensors_CommandLine(uint16_t argc, uint8_t **argv);
static HAL_StatusTypeDef Leds_CommandLine(uint16_t argc, uint8_t **argv);

//==============================================================================
// SOURCE CODE
//==============================================================================

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

	if (strcmp((const char *) "rtos", (const char *) cmd) == 0)
	{
		RTOS_DBG_ShowStatus();
	}
	if (strcmp((const char *) "help", (const char *) cmd) == 0)
	{
		Help_Commads();
	}
	else if (strcmp((const char *) "get", (const char *) cmd) == 0)
	{
		Sensors_CommandLine(argc, argv);
	}
	else if (strcmp((const char *) "led", (const char *) cmd) == 0)
	{
		Leds_CommandLine(argc, argv);
	}
}
