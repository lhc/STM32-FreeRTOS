/**
 * @file    setup_hw.h
 * @author  Jorge Guzman,
 * @date    Fev 2, 2018
 * @version 0.1.0
 * @brief   Biblioteca que contem as rotinas de configuracao e inicializacao do projeto
 * @details
 */

//==============================================================================
// INCLUDE FILES
//==============================================================================

#include "setup_hw.h"
#include "setup_debug.h"
#include "sensores.h"

#include "leds/leds.h"
#include "hts221/hts221.h"
#include "lps22hb/lps22hb.h"
#include "lsm6dsl/lsm6dsl.h"
#include "lis3mdl/lis3mdl.h"
#include "micro-shell/micro-shell.h"

//==============================================================================
// PRIVATE DEFINITIONS
//==============================================================================

#define SHELL_MULTIPLY_TASK_SIZE    8       /* Tamanho da task shell = configMINIMAL_STACK_SIZE x SHELL_MULTIPLY_STACK_SIZE */

//==============================================================================
// EXTERN VARIABLES
//==============================================================================

extern UART_HandleTypeDef huart1;
extern I2C_HandleTypeDef hi2c2;

//==============================================================================
// PRIVATE VARIABLES
//==============================================================================

//==============================================================================
// PRIVATE FUNCTIONS
//==============================================================================

/** @brief Start libraries of middlware */
static void Setup_InitMiddlware(void);

/** @brief Start Apps of application */
static void Setup_InitApps(void);

//==============================================================================
// SOURCE CODE
//==============================================================================

static void Setup_InitMiddlware(void)
{
    /* Inicializa Led */
	Leds_Attach(N_LED1, GPIOB, GPIO_PIN_14, LED_ATIVE_HIGH);

	HTS221_attach(&hi2c2);
	HTS221_Init(HTS221_I2C_ADDRESS);

	LPS22HB_attach(&hi2c2);
	LPS22HB_Init(LPS22HB_I2C_ADDRESS);

	LSM6DSL_attach(&hi2c2);
	LSM6DSL_myInit();

	LIS3MDL_attach(&hi2c2);
	LIS3MDL_myInit();

	/* Inicializa recepcao de dado pela serial */
	Debug_RX_Init(&huart1);
}

static void Setup_InitApps(void)
{
	Sensors_t sens = {0};

	DBG("%s[ OK ]%s\t %s\r\n", ANSI_COLOR_GREEN, DEF_CONSOLE_DEFAULT, "Mensagem Colorida");

	Sensores_Read(&sens);
	Sensor_Print(&sens);
	Sensor_Print_SerialPlot(&sens);

	/* Inicializa task shell */
	Shell_TaskInit(SHELL_MULTIPLY_TASK_SIZE);

	Leds_TaskInit();
	Leds_Set(N_LED1, LED_BLINK_HEARTBEAT);

}

void Setup_Init(void)
{
	//==============================================================================
	// Inicializa Drivers usados pelas Libs
	//==============================================================================

	Setup_InitMiddlware();

	//==============================================================================
	// Inicializa Tasks da Aplicacao
	//==============================================================================

	Setup_InitApps();

	/* Envia mensagem de start do sistema */
	DBG("Init Program...");
}

//==============================================================================


