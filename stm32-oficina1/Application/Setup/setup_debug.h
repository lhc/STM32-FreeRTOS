/**
 * @file    setup_debug.h
 * @author  Jorge Guzman
 * @date    May 27, 2018
 * @version 0.1.0
 * @brief   Lib usa rotinas de debug
 */

#ifndef _SETUP_DEBUG_H_
#define _SETUP_DEBUG_H_


/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif

//==============================================================================
// INCLUDE FIRST FILES
//==============================================================================

#include "setup_hw.h"
#include <stdarg.h>
#include <stdio.h>

//==============================================================================
// PUBLIC DEFINICTIONS
//==============================================================================

#define ANSI_COLOR_RED              "\x1b[31m"
#define ANSI_COLOR_GREEN            "\x1b[32m"
#define ANSI_COLOR_YELLOW           "\x1b[33m"
#define ANSI_COLOR_BLUE             "\x1b[34m"
#define ANSI_COLOR_MAGENTA          "\x1b[35m"
#define ANSI_COLOR_CYAN             "\x1b[36m"
#define ANSI_COLOR_RESET            "\x1b[0m"

#define DEF_CONSOLE_CLEAR           "\033[2J\033[1;1H"
#define DEF_CONSOLE_DEFAULT         "\033[0m"

#define DEF_CONSOLE_RED             "\033[91m"
#define DEF_CONSOLE_GREEN           "\033[92m"
#define DEF_CONSOLE_YELLOW          "\033[93m"
#define DEF_CONSOLE_BLUE            "\033[94m"
#define DEF_CONSOLE_WHITE           "\033[97m"
#define DEF_CONSOLE_BOLD_RED        "\033[1;31m"
#define DEF_CONSOLE_BOLD_YELLOW     "\033[1;33m"
#define DEF_CONSOLE_BOLD_BLUE       "\033[1;34m"
#define DEF_CONSOLE_BOLD_GREEN      "\033[1;32m"
#define DEF_CONSOLE_BOLD_WHITE      "\033[1;37m"

#define DEF_BACKGROUND_BLACK        "\033[40;1;37m"
#define DEF_BACKGROUND_RED          "\033[41;1;37m"
#define DEF_BACKGROUND_GREEN        "\033[42;1;37m"
#define DEF_BACKGROUND_BROWN        "\033[43;1;37m"
#define DEF_BACKGROUND_BLUE         "\033[44;1;37m"
#define DEF_BACKGROUND_PURBLE       "\033[45;1;37m"
#define DEF_BACKGROUND_CYAN         "\033[46;1;37m"
#define DEF_BACKGROUND_GRAY         "\033[47;1;37m"

#define DBG(fmt, ...)               Debug_Printf(fmt"\r\n", ##__VA_ARGS__)
#define DBG_BKPT()                  asm("BKPT #0")
#define SHELL_PRINTF(fmt, ...)      Debug_Printf(fmt"\r\n", ##__VA_ARGS__)
#define DBG_ASSERT_PARAM(expr)     ((expr) ? (void)0U : Debug_AssertFailed(__FILE__, __LINE__))

//==============================================================================
// PUBLIC FUNCTIONS
//==============================================================================


uint8_t Debug_Get_Data(void);
void Debug_RX_Init(UART_HandleTypeDef *pUart);
HAL_StatusTypeDef Debug_Printf(char * format, ...);
void Debug_AssertFailed(const char* s8File, int s16Line);


/* C++ detection */
#ifdef __cplusplus
}
#endif


#endif
