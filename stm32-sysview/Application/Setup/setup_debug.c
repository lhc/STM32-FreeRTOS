/**
 * @file    setup_debug.h
 * @author  Jorge Guzman
 * @date    May 27, 2018
 * @version 0.1.0
 * @brief   Lib usa rotinas de debug
 */


//==============================================================================
// INCLUDE FILES
//==============================================================================

#include "setup_debug.h"
#include "micro-shell/micro-shell.h"

//==============================================================================
// PRIVATE DEFINITIONS
//==============================================================================

#define SERIAL_BUFFER_SIZE          500

//==============================================================================
// PRIVATE TYPEDEFS
//==============================================================================

//==============================================================================
// EXTERN VARIABLES
//==============================================================================

UART_HandleTypeDef *pUartDebug;

//==============================================================================
// PRIVATE VARIABLES
//==============================================================================

static uint8_t bufferSerial[SERIAL_BUFFER_SIZE];
volatile uint8_t dataRX;  /* byte usado para receber os dados vindos da serial via DMA */
SemaphoreHandle_t mutex_debug;

//==============================================================================
// PRIVATE FUNCTIONS
//==============================================================================

//==============================================================================
// SOURCE CODE
//==============================================================================

void Debug_AssertFailed(const char* s8File, int s16Line)
{
	DBG_BKPT();
	DBG("Wrong parameters value: file %s on line %d\r\n", s8File, s16Line);

	/* In debug mode */
	if (( CoreDebug->DHCSR & 0x1 ) != 0x1)
	{
		for (;;)
		{

		}
	}
}

void Debug_RX_Init(UART_HandleTypeDef *pUart)
{
	pUartDebug = pUart;

	/* Habilita recepccao serial usando DMA Circular de 1 byte.
	 * Obs: Este metodo evita o rearme da recepcao de dados dentro da interrupcao. */
	HAL_UART_Receive_DMA(pUartDebug, (uint8_t*)&dataRX, 1);

	if(mutex_debug == NULL)
	{
		mutex_debug = xSemaphoreCreateMutex();

		DBG_ASSERT_PARAM(mutex_debug);
	}

}

uint8_t Debug_Get_Data(void)
{
	return dataRX;
}

HAL_StatusTypeDef Debug_Printf(char * format, ...)
{
	va_list args;
	uint16_t len;
	HAL_StatusTypeDef resp = HAL_ERROR;

	if( xSemaphoreTake(mutex_debug, 1000) == pdTRUE )
	{
		va_start(args, format);
		len = vsnprintf((char *) bufferSerial, SERIAL_BUFFER_SIZE, format, args);
		va_end(args);

		resp = HAL_UART_Transmit(pUartDebug, bufferSerial, len, 1000);

		xSemaphoreGive(mutex_debug);
	}
	return resp;
}

