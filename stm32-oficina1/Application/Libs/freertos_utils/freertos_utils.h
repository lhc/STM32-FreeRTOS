/**
 * @file    freertos_debug.h
 * @author  Jorge Guzman,
 * @date    Mai 23, 2018
 * @version 0.1.0.
 * @brief   Biblioteca para monitorar o status da CPU usando FreeRTOS.
 * @details
 */

#ifndef __CPU_UTILS_H_
#define __CPU_UTILS_H_

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif

//==============================================================================
// INCLUDE FILES
//==============================================================================

#include <stdint.h>

//==============================================================================
// PUBLIC FUNCTIONS
//==============================================================================

/** @brief Function the print in the serial debug the status of FreeRTOS. */
uint8_t *RTOS_DBG_ShowStatus(void);
void RTOS_TimerAttachStatics20Khz(void *pTimer20Khz);
void RTOS_TimerStart20Khz(void);
void RTOS_TimerStop20Khz(void);
void RTOS_ISR_Timer20Khz(void);

/* C++ detection */
#ifdef __cplusplus
}
#endif

#endif /* __CPU_UTILS_H_ */

