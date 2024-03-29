/**
 * @file    micro-shell.h
 * @brief   Biblioteca para o uso do micro-shell
 */

#ifndef _MICRO_SHELL_H
#define _MICRO_SHELL_H

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif

//==============================================================================
// INCLUDE FILES
//==============================================================================

#include "setup_hw.h"
#include <stdint.h>

//==============================================================================
// PUBLIC DEFINITIONS
//==============================================================================

//==============================================================================
// PUBLIC TYPEDEFS
//==============================================================================

//==============================================================================
// PUBLIC VARIABLES
//==============================================================================

//==============================================================================
// PUBLIC FUNCTIONS
//==============================================================================

/**
 * Function to create the task micro-shell
 * @param multi_stack_size value multiplied by configMINIMAL_STACK_SIZE
 */
void Shell_TaskInit(uint16_t multi_stack_size);

/**
 * Function to receive one byte
 * @param c byte received.
 */
void Shell_Getc(uint8_t c);

/**
 * Function to receive one byte from an interrupt.
 * @param c Byte received.
 * @param pHigherPriorityTaskWoken Woken of task.
 */
void Shell_ISR_Getc(uint8_t c, BaseType_t *pHigherPriorityTaskWoken);

//==============================================================================
// WEAK FUNCTIONS
//==============================================================================

/**
 * Weak function to handle received command.
 * @param cmd String raw.
 * @param argc Number of arguments.
 * @param argv String with arguments.
 */
void Shell_Callback(uint8_t *cmd, uint16_t argc, uint8_t **argv);

/* C++ detection */
#ifdef __cplusplus
}
#endif

#endif /* _MICRO_SHELL_H */
