/**
 * @file    leds.h
 * @author  Jorge Guzman
 * @date    Jan 14, 2015
 * @version 0.1.0
 * @brief   Bibliteoca para o uso dos Leds
 */

#ifndef _LEDS_H_
#define _LEDS_H_

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif

//==============================================================================
// CHECK TO ENABLE LIBRARY
//==============================================================================

#include "setup_hw.h"

//==============================================================================
// INCLUDE FILES
//==============================================================================

//==============================================================================
// PRIVATE DEFINITIONS
//==============================================================================

//==============================================================================
// PUBLIC DEFINITIONS
//==============================================================================

/** @brief Acoes que o led pode realizar*/
typedef enum
{
	LED_OFF = 0, 		/**< Desliga o led*/
	LED_ON, 			/**< Liga o led*/
	LED_BLINK_SLOW, 	/**< Faz o led piscar lentamente */
	LED_BLINK_FAST, 	/**< Faz o led piscar rapidamente */
	LED_BLINK_HEARTBEAT /**< Faz o led piscar na forma de hearbeat */
} LedsAction_e;

typedef enum
{
	LED_ATIVE_LOW = 0,
	LED_ATIVE_HIGH,
} LedActive_e;

//==============================================================================
// PUBLIC TYPEDEFS
//==============================================================================

/** @brief Representação de todos os led para uso nas funcaos leds_set e leds_attach.*/
typedef enum
{
	N_LED1 = 1 << 0, /**< Representacao em byte do led 1 */
	N_LED2 = 1 << 1, /**< Representacao em byte do led 2 */
	N_LED3 = 1 << 2, /**< Representacao em byte do led 3 */
	N_LED4 = 1 << 3, /**< Representacao em byte do led 4 */
	N_LED5 = 1 << 4, /**< Representacao em byte do led 5 */
	N_LED6 = 1 << 5, /**< Representacao em byte do led 6 */
	N_LED7 = 1 << 6, /**< Representacao em byte do led 7 */
	N_LED8 = 1 << 7, /**< Representacao em byte do led 8 */
	ALL_LED = 0xFF,
} LedsIndex_e;

typedef struct
{
	LedsIndex_e index;
	LedsAction_e action;
}MsgLed_t;

//==============================================================================
// EXTERN VARIABLES
//==============================================================================

//==============================================================================
// PUBLIC FUNCTIONS
//==============================================================================

/**
 * Limpa o vetor que armazena pinos I/O que serao usados pela lib, inicializa
 * as variaveis auxiliares e inicializa task.
 */
void Leds_TaskInit(void);

/**
 * Funcao para o controle do acendimento dos leds.
 * @param in_leds indica os leds que serao usados.
 * @param action acao que os leds indicados deverao realizar.
 */
void Leds_Set(LedsIndex_e in_leds, LedsAction_e action);

/**
 * Funcao que configura um led atribuindo a ele uma representação de pino I/O.
 * @param Led que sera configurado (Range: N_LED1 a N_LED8).
 * @param pin Pino I/O que deseja se representar.
 */
void Leds_Attach(LedsIndex_e index, GPIO_TypeDef *gpio, uint16_t pin, LedActive_e active);

/* C++ detection */
#ifdef __cplusplus
}
#endif

#endif /* _LEDS_H_ */
