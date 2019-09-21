/**
 * @file    freertos_debug.c
 * @author  Jorge Guzman,
 * @date    Jul 23, 2017
 * @version 0.1.0.
 * @brief   Biblioteca para monitorar o status do FreeRTOS
 * @details
 * Macros que precisam estar habilitadas no freeRTOS
 * configUSE_IDLE_HOOK  1 (Habilita a funcao vApplicationIdleHook)
 */

//==============================================================================
// CHECK TO ENABLE LIBRARY
//==============================================================================
#include "setup_hw.h"

#if defined(HAL_TIM_MODULE_ENABLED) & \
        (configGENERATE_RUN_TIME_STATS > 0) & \
        (configUSE_STATS_FORMATTING_FUNCTIONS > 0) & \
        (configUSE_TRACE_FACILITY > 0)

//==============================================================================
// INCLUDE FILES
//==============================================================================

//==============================================================================
// PRIVATE DEFINITIONS
//==============================================================================

//==============================================================================
// PRIVATE VARIABLES
//==============================================================================

/** @brief Incrementar variavel dentro de uma interrupcao de 2Khz=500us */
volatile unsigned long  ulHighFrequencyTimerTicks;

TIM_HandleTypeDef *pxTimer20Khz = NULL;

//==============================================================================
// PRIVATE FUNCTIONS
//==============================================================================

//==============================================================================
// PRIVATE FUNCTIONS
//==============================================================================

//==============================================================================
// PUBLIC SOURCE CODE
//==============================================================================

void RTOS_TimerAttachStatics20Khz(void *pTimer20Khz)
{
    pxTimer20Khz = ( TIM_HandleTypeDef* )pTimer20Khz;
    DBG_ASSERT_PARAM( pxTimer20Khz );
}

void RTOS_TimerStart20Khz(void)
{
    if( pxTimer20Khz == 0)
    {
    	for(;;)
    	{

    	}
    }
    HAL_TIM_Base_Start_IT ( pxTimer20Khz );
}

void RTOS_TimerStop20Khz(void)
{
    DBG_ASSERT_PARAM( pxTimer20Khz );
    HAL_TIM_Base_Stop( pxTimer20Khz );
}

void RTOS_ISR_Timer20Khz(void)
{
    ulHighFrequencyTimerTicks++;
}

unsigned long getRunTimeCounterValue(void)
{
    return ulHighFrequencyTimerTicks;
}

void configureTimerForRunTimeStats(void)
{
    ulHighFrequencyTimerTicks = 0ul;
    RTOS_TimerStart20Khz();
}

#else

__weak void RTOS_TimerAttachStatics20Khz(void *pTimer20Khz)
{

}

__weak void RTOS_TimerStart20Khz(void)
{

}

__weak void RTOS_TimerStop20Khz(void)
{

}

__weak void RTOS_ISR_Timer20Khz(void)
{

}

#endif

