/**
 * @file    setup_hw_isr.c
 * @author  Jorge Guzman
 * @date    May 27, 2018
 * @version 0.1.0
 * @brief   Lib faz o uso da chamada de callbacks usadas pelas interrucoes da aplicacao.
 */

//==============================================================================
// INCLUDE FILES
//==============================================================================

#include "setup_hw_isr.h"
#include "setup_hw.h"
#include "setup_debug.h"
#include "micro-shell/micro-shell.h"

//==============================================================================
// PRIVATE DEFINITIONS
//==============================================================================

//==============================================================================
// PRIVATE TYPEDEFS
//==============================================================================

//==============================================================================
// EXTERN VARIABLES
//==============================================================================

extern UART_HandleTypeDef huart1;

//==============================================================================
// PRIVATE FUNCTIONS
//==============================================================================

//==============================================================================
// SOURCE CODE
//==============================================================================

void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *uart)
{
	uint8_t data;
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;

	if(uart->Instance == USART1)
	{
		data = Debug_Get_Data();

		Shell_ISR_Getc(data, &xHigherPriorityTaskWoken);
	}

	if( xHigherPriorityTaskWoken == pdTRUE )
	{
		portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
	}
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *uart)
{
	if(uart->Instance == USART1)
	{
		Debug_RX_Init(&huart1);
	}
}

