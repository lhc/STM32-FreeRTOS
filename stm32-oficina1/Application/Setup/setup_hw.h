/**
 * @file    setup_hw.h
 * @author  Jorge Guzman,
 * @date    Fev 2, 2018
 * @version 0.1.0
 * @brief   Biblioteca que contem as rotinas de configuracao e inicializacao do projeto
 * @details
 */

#ifndef _SETUP_HW_
#define	_SETUP_HW_

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif

//==============================================================================
// INCLUDE FIRST FILES
//==============================================================================

#include "main.h"
#include "stm32l4xx_hal.h"
#include "cmsis_os.h"

#include "setup_debug.h"
#include <stdint.h>
#include <stdarg.h>

//==============================================================================
// PUBLIC DEFINICTIONS
//==============================================================================

//==============================================================================
// PUBLIC FUNCTIONS
//==============================================================================

void Setup_Init(void);


/* C++ detection */
#ifdef __cplusplus
}
#endif

#endif
