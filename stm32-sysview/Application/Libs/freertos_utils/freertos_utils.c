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
// INCLUDE FILES
//==============================================================================

#include "setup_hw.h"
#include "task.h"

//==============================================================================
// PRIVATE DEFINITIONS
//==============================================================================

#define SIZE_RTOS_BUFF			1024
#define heap_threshold  		(configTOTAL_HEAP_SIZE/10)*2

//==============================================================================
// PRIVATE VARIABLES
//==============================================================================

//==============================================================================
// PRIVATE FUNCTIONS
//==============================================================================

//==============================================================================
// APIS OF FREERTOS
//==============================================================================

#if configUSE_IDLE_HOOK > 0
void vApplicationIdleHook(void)
{
	for (;;)
	{

	}
}
#endif /* configUSE_IDLE_HOOK */


#if configUSE_MALLOC_FAILED_HOOK > 0
void vApplicationMallocFailedHook(void)
{
#if defined(USE_SYSVIEW)
	SEGGER_SYSVIEW_ErrorfTarget("(X) Malloc Failed");
#endif

	DBG_BKPT();
	DBG("(X) vApplicationMallocFailedHook");
	for(;;)
	{

	}
}
#endif /* configUSE_MALLOC_FAILED_HOOK */


#if configCHECK_FOR_STACK_OVERFLOW > 0
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
#if defined(USE_SYSVIEW)
	SEGGER_SYSVIEW_ErrorfTarget("(X) StackOverflowHook: %s", pcTaskName);
#endif

	DBG_BKPT();
	( void ) pcTaskName;
	( void ) xTask;
	DBG("(X) Stack overflow: [%s]", pcTaskName);
	for(;;)
	{

	}
}
#endif /* configCHECK_FOR_STACK_OVERFLOW */


#if configUSE_TICK_HOOK > 0

void vApplicationTickHook(void)
{

}

#endif

//==============================================================================
// PUBLIC SOURCE CODE
//==============================================================================

#if (configGENERATE_RUN_TIME_STATS > 0) && (configUSE_STATS_FORMATTING_FUNCTIONS > 0) && (configUSE_TRACE_FACILITY > 0)

/**
 * @brief Buffer que contem as estatisticas do FreeRTOS
 */
static uint8_t pcWriteBuffer[SIZE_RTOS_BUFF];

uint8_t *RTOS_DBG_ShowStatus(void)
{
	uint16_t heap_curr;
	uint32_t cpu_clock;
	uint32_t ticks_hz;

	DBG("=================================================");
	DBG("\t\tCPU");
	DBG("=================================================");
	cpu_clock = configCPU_CLOCK_HZ;
	ticks_hz = configTICK_RATE_HZ;
	DBG("Clock CPU  : %li Hz", cpu_clock);
	DBG("RTOS ticks : %li Hz", ticks_hz);

	DBG("=================================================");
	DBG("\t\tHEAP");
	DBG("=================================================");
	heap_curr = xPortGetFreeHeapSize();

	DBG("Size heap: %i", configTOTAL_HEAP_SIZE);
	DBG("Free heap: %d", heap_curr);

	if (heap_curr < heap_threshold)
	{
		DBG("(X) Heap perto do limite");
	}

	/* Task List */
	DBG("=================================================");
	DBG("Task\t\tState\tPrio\tStack\tNum");
	DBG("=================================================");
	vTaskList((char *)&pcWriteBuffer);
	DBG("%s", pcWriteBuffer);

	/* RTOS Run-Time Statistics */
	DBG("=================================================");
	DBG("Task\t\tAbs Time\t%c Time ", 0x25);
	DBG("=================================================");
	vTaskGetRunTimeStats((char *)&pcWriteBuffer);
	DBG("%s", pcWriteBuffer);

	return pcWriteBuffer;
}

#else

__weak void RTOS_DBG_ShowStatus(void)
{

}

#endif


