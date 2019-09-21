/**
 * @file    leds.c
 * @author  Jorge Guzman
 * @date    Apr 23, 2014
 * @version 0.3.0
 * @brief   Bibliteoca para o uso dos Leds
 */

//==============================================================================
// CHECK TO ENABLE LIBRARY
//==============================================================================

#include "leds.h"

//==============================================================================
// INCLUDE FILES
//==============================================================================

#include "bitwise/bitwise.h"
#include <stddef.h>

//==============================================================================
// PRIVATE DEFINITIONS
//==============================================================================

/** @brief intervalo de tempo x10ms de delay para gerar o blink slow*/
#define configLED_BLINK_SLOW_STEP 			70

/** @brief intervalo de tempo x10ms de delay para gerar o blink fast*/
#define configLED_BLINK_FAST_STEP 			20

/** @brief intervalos de tempo x10ms de delay para gerar o hearbeat fast*/
#define configLED_BLINK_HEARBEAT_STATE_1 	10
#define configLED_BLINK_HEARBEAT_STATE_2 	20
#define configLED_BLINK_HEARBEAT_STATE_3 	10
#define configLED_BLINK_HEARBEAT_STATE_4 	120

/** @brief Numero total de leds*/
#define configMAX_NUM_LEDS 					8

//==============================================================================
// PRIVATE TYPEDEFS
//==============================================================================

typedef struct
{
	GPIO_TypeDef *gpio;
	uint16_t pin;
	LedActive_e active;
} gpio_leds;

//==============================================================================
// PRIVATE VARIABLES
//==============================================================================

/** @brief Timer used to control the leds status */
TimerHandle_t timerHendlerLeds;

/**@brief Armazena os pinos I/O que serao usados. */
gpio_leds leds[configMAX_NUM_LEDS] = {0};

/** @brief Controle imediato do estado das leds */
uint8_t ledsStatus;

/** @brief Indicao das leds que estao 100% ligadas */
uint8_t ledsON;

/** @brief Indicao das Leds que estao picando lentamnete */
uint8_t leds_blink_slow = 0;

/** @brief Indicao das Leds que estao piscando rapido */
uint8_t leds_blink_fast = 0;

/** @brief Indicao das Leds que estao piscando  */
uint8_t leds_blink_heartbeat = 0;

QueueHandle_t QueueLeds = NULL;

//==============================================================================
// PRIVATE FUNCTIONS
//==============================================================================

/**
 * Verifica quais leds serao ligados
 * @param leds_mask: mascara que contem quais leds serao ligados.
 */
static void Leds_On(uint8_t leds_mask);

/**
 * Verifica quais leds serao desligados
 * @param leds_mask: mascara que contem quais leds serao desligados
 */
static void Leds_Off(uint8_t leds_mask);

/**
 * Escreve o valor de saida
 * @param out Valor de saida representado por bits
 */
static void Leds_Write(uint8_t out);

/*
 * Funcao para executar uma acao para um determinado led.
 * @param in_leds Indice do led
 * @param action Acao que o led ira executar
 */
static void Leds_Action(LedsIndex_e in_leds, LedsAction_e action);

/*
 * Task principal da rotina de blink dos leds.
 */
static void Leds_Task(void *param);

/**
 * Inverte os leds representados pelos bits 1 da mascara de entrada
 * @param leds_mask: mascara que contem quais leds serao invertidos
 */
static void Leds_Reverse(uint8_t leds_mask);

/**
 * Funcao usada para monitoramento do status dos leds.
 * @return retorna status dos leds
 */
static uint8_t Leds_Status(void);

//==============================================================================
// PRIVATE SOURCE CODE
//==============================================================================

static void Leds_Reverse(uint8_t leds_mask)
{
	/* auxiliar para operacaes logicas */
	uint8_t leds_temp;

	/* verifica se a mascara tem algum bit ativo */
	if (leds_mask)
	{
		/* Tem, entao executa a operacao de inversao */
		/* copia o status para var temp */
		leds_temp = ledsStatus;

		/* ativa os bits definidos na mascara */
		Leds_On(leds_mask);

		/* verifica se o valor e igual ao anterior, isso significa que os bits ja estava ligados */
		if (leds_temp == ledsStatus)
		{
			/* entao devemos desligar os bits, pois eles ja estavam ativos */
			Leds_Off(leds_mask);
		}
	}
}

static uint8_t Leds_Status(void)
{
	uint8_t index;
	uint8_t status;
	status = 0;

	for (index = 0; index < configMAX_NUM_LEDS; index++)
	{
		if ((leds[index].gpio != NULL) && (HAL_GPIO_ReadPin(leds[index].gpio, leds[index].pin) == GPIO_PIN_SET))
		{
			if (leds[index].active == LED_ATIVE_HIGH)
			{
				status |= (1 << index);
			}
		}
	}
	return (status);
}

static void Leds_On(uint8_t leds_mask)
{
	uint8_t aux;
	/* Executa um OU para ativar os bits */
	ledsStatus = ledsStatus | leds_mask;

	/* Atualiza os LEDs no frontal */
	aux = Leds_Status();

	aux |= ledsStatus;

	Leds_Write(aux);
}

static void Leds_Off(uint8_t leds_mask)
{
	uint8_t aux;
	uint8_t leds_temp;

	/* devemos desligar os bits */
	leds_temp = ledsStatus ^ leds_mask;

	ledsStatus = ledsStatus & leds_temp;

	aux = ledsStatus;

	aux &= ~leds_mask;

	/* Atualiza os LEDs no frontal */
	Leds_Write(aux);
}

static void Leds_Write(uint8_t out)
{
	uint8_t index;

	for (index = 0; index < configMAX_NUM_LEDS; index++)
	{
		if (leds[index].gpio != NULL)
		{
			if (tst_bit(out, index))
			{
				HAL_GPIO_WritePin(leds[index].gpio, leds[index].pin, (leds[index].active == LED_ATIVE_HIGH ? GPIO_PIN_SET : GPIO_PIN_RESET));
			}
			else
			{
				HAL_GPIO_WritePin(leds[index].gpio, leds[index].pin, (leds[index].active == LED_ATIVE_HIGH ? GPIO_PIN_RESET : GPIO_PIN_SET));
			}
		}
	}
}

static void Leds_Run()
{
	static uint8_t state_hearbeat;
	static uint8_t timer_led_hearbeat_reverse_counter = 1;
	static uint8_t timer_led_slow_reverse_counter = 0; /* variavel de controle do blink lento */
	static uint8_t timer_led_fast_reverse_counter = 0; /* controle do blink rapido */

	/* soma 1 nos contadores */
	timer_led_slow_reverse_counter++;
	timer_led_fast_reverse_counter++;
	timer_led_hearbeat_reverse_counter--;

	/* verifica se devemos fazer a reversao para os slow blinks */
	if (timer_led_slow_reverse_counter == configLED_BLINK_SLOW_STEP)
	{
		/* sim, o contador do slow estourou, fazemos a reversao */
		Leds_Reverse(leds_blink_slow);

		/* zera o valor do contador apos a reversao */
		timer_led_slow_reverse_counter = 0;
	}

	/* verifica se devemos fazer a reversao para os fast blinks */
	if (timer_led_fast_reverse_counter == configLED_BLINK_FAST_STEP)
	{
		/* sim, o contador do slow estourou, fazemos a reversao */
		Leds_Reverse(leds_blink_fast);

		/* zera o valor do contador apos a reversao */
		timer_led_fast_reverse_counter = 0;
	}

	/* verifica se devemos fazer a reversao para os hearbeat blinks */
	if (timer_led_hearbeat_reverse_counter == 0)
	{
		switch (state_hearbeat)
		{
		case 0:
			timer_led_hearbeat_reverse_counter = configLED_BLINK_HEARBEAT_STATE_1;
			state_hearbeat++;
			break;

		case 1:
			timer_led_hearbeat_reverse_counter = configLED_BLINK_HEARBEAT_STATE_2;
			state_hearbeat++;
			break;

		case 2:
			timer_led_hearbeat_reverse_counter = configLED_BLINK_HEARBEAT_STATE_3;
			state_hearbeat++;
			break;

		default:
			timer_led_hearbeat_reverse_counter = configLED_BLINK_HEARBEAT_STATE_4;
			state_hearbeat = 0;
			break;
		}

		/* sim, o contador do slow estourou, fazemos a reversao */
		Leds_Reverse(leds_blink_heartbeat);
	}
}

static void Leds_Task(void *param)
{

	QueueHandle_t *pQueue;

	pQueue = (QueueHandle_t*)param;
	MsgLed_t MessageLed;

	for(;;)
	{
		/* Verifica se recebeu algum comando */
		if(xQueueReceive(pQueue, &MessageLed, 10) == pdTRUE)
		{
			Leds_Action(MessageLed.index, MessageLed.action);
		}
		else
		{
			Leds_Run();
		}
	}
}

static void Leds_Action(LedsIndex_e in_leds, LedsAction_e action)
{
	/* auxiliar para operacoes logicas */
	uint8_t leds_temp;

	/* verifica qual acao que as leds em questao deve executar */
	switch (action)
	{
	case LED_OFF:

		/* XOR seguido de um AND para desligar os bits */
		/* Apaga as leds 100% ligadas */
		leds_temp = ledsON ^ in_leds;
		ledsON = ledsON & leds_temp;

		/* apaga as leds com blink lento */
		leds_temp = leds_blink_slow ^ in_leds;
		leds_blink_slow = leds_blink_slow & leds_temp;

		/* apaga as leds com blink rapido */
		leds_temp = leds_blink_fast ^ in_leds;
		leds_blink_fast = leds_blink_fast & leds_temp;

		/* apaga as leds com blink heartbeat */
		leds_temp = leds_blink_heartbeat ^ in_leds;
		leds_blink_heartbeat = leds_blink_heartbeat & leds_temp;

		/* apaga as leds da mascara */
		Leds_Off(in_leds);

		break;

	case LED_ON:

		/* devemos ascender as leds em questao */
		ledsON = ledsON | in_leds;

		/* desliga as leds com blink lento */
		leds_temp = leds_blink_slow ^ in_leds;
		leds_blink_slow = leds_blink_slow & leds_temp;

		/* desliga as leds com blink rapido */
		leds_temp = leds_blink_fast ^ in_leds;
		leds_blink_fast = leds_blink_fast & leds_temp;

		/* apaga as leds com blink heartbeat */
		leds_temp = leds_blink_heartbeat ^ in_leds;
		leds_blink_heartbeat = leds_blink_heartbeat & leds_temp;

		/* acende as leds definidas */
		Leds_On(ledsON);

		break;

	case LED_BLINK_SLOW:

		/* Apaga as leds 100% ligadas */
		leds_temp = ledsON ^ in_leds;
		ledsON = ledsON & leds_temp;

		/* ativa as leds com blink lento */
		leds_blink_slow = leds_blink_slow | in_leds;

		/* apaga as leds com blink rapido */
		leds_temp = leds_blink_fast ^ in_leds;
		leds_blink_fast = leds_blink_fast & leds_temp;

		/* apaga as leds com blink heartbeat */
		leds_temp = leds_blink_heartbeat ^ in_leds;
		leds_blink_heartbeat = leds_blink_heartbeat & leds_temp;

		break;

	case LED_BLINK_FAST:

		/* Apaga as leds 100% ligadas */
		leds_temp = ledsON ^ in_leds;
		ledsON = ledsON & leds_temp;

		/* apaga as leds com blink lento */
		leds_temp = leds_blink_slow ^ in_leds;
		leds_blink_slow = leds_blink_slow & leds_temp;

		/* ativa as leds com blink rapido */
		leds_blink_fast = leds_blink_fast | in_leds;

		/* apaga as leds com blink heartbeat */
		leds_temp = leds_blink_heartbeat ^ in_leds;
		leds_blink_heartbeat = leds_blink_heartbeat & leds_temp;

		break;

	case LED_BLINK_HEARTBEAT:

		/* apaga as leds com blink lento */
		leds_temp = leds_blink_slow ^ in_leds;
		leds_blink_slow = leds_blink_slow & leds_temp;

		/* apaga as leds com blink rapido */
		leds_temp = leds_blink_fast ^ in_leds;
		leds_blink_fast = leds_blink_fast & leds_temp;

		/* ativa as leds com blink heartbeat */
		leds_blink_heartbeat = leds_blink_fast | in_leds;

		break;

	default:
		break;

	}
}
//==============================================================================
//  SOURCE CODE
//==============================================================================

void Leds_TaskInit(void)
{
	BaseType_t xReturned;

	ledsStatus = 0;
	ledsON = 0;
	leds_blink_slow = 0;
	leds_blink_fast = 0;

	QueueLeds = xQueueCreate(4, sizeof(MsgLed_t));
	DBG_ASSERT_PARAM(QueueLeds);
	vQueueAddToRegistry(QueueLeds, (const char *)"Leds");

	xReturned = xTaskCreate(Leds_Task, "LedsTimer", configMINIMAL_STACK_SIZE * 2, QueueLeds, 3, NULL);
	configASSERT(xReturned);

}

void Leds_Set(LedsIndex_e in_leds, LedsAction_e action)
{
	MsgLed_t msgLed;

	msgLed.index = in_leds;
	msgLed.action = action;

	xQueueSend(QueueLeds, &msgLed, 1000);
}


void Leds_Attach(LedsIndex_e index, GPIO_TypeDef *gpio, uint16_t pin, LedActive_e active)
{
	leds[countUntilFirstByteOne(index)].gpio = gpio;
	leds[countUntilFirstByteOne(index)].pin = pin;
	leds[countUntilFirstByteOne(index)].active = active;
}

