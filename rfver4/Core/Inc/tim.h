#ifndef TIM_H
#define TIM_H

#include "directive.h"

#define TIM_1SEC_ARR 8000
#define TIM_1SEC_PSC (CPU_CLOCK_MHZ* FREQUENCY_MHZ / TIM_1SEC_ARR)

#define TIM1_MSEC_PSK (CPU_CLOCK_MHZ* FREQUENCY_KHZ)

#define TIM_10_MSEC_ARR 8000
#define TIM_100_MSEC_PSC (CPU_CLOCK_MHZ* FREQUENCY_MHZ /10/ TIM3_1SEC_ARR)


#define TIM1_DISABLE() TIM1->CR1 &= ~TIM_CR1_CEN 	/* отключение таймера 2*/
#define TIM1_ENABLE() TIM1->CR1 |= TIM_CR1_CEN 		/* включение таймера 2*/
#define TIM2_DISABLE() TIM2->CR1 &= ~TIM_CR1_CEN 	/* отключение таймера 2*/
#define TIM2_ENABLE() TIM2->CR1 |= TIM_CR1_CEN 		/* включение таймера 2*/
#define TIM3_DISABLE() TIM3->CR1 &= ~TIM_CR1_CEN 	/* отключение таймера 3*/
#define TIM3_ENABLE() TIM3->CR1 |= TIM_CR1_CEN 		/* включение таймера 3*/
#define TIM4_DISABLE() TIM4->CR1 &= ~TIM_CR1_CEN 	/* отключение таймера 4*/
#define TIM4_ENABLE() TIM4->CR1 |= TIM_CR1_CEN 		/* включение таймера 4*/
#define TIM17_DISABLE() TIM17->CR1 &= ~TIM_CR1_CEN 	/* отключение таймера 17*/
#define TIM17_ENABLE() TIM17->CR1 |= TIM_CR1_CEN 	/* включение таймера 17*/

extern uint8_t global_main_flag_tim3_update;
extern uint8_t global_main_flag_tim2_update;
extern uint8_t global_main_flag_tim1_update;
extern uint16_t global_relative_time;
void TIM3_IRQHandler (void);
void TIM2_IRQHandler (void);
void TIM1_UP_IRQHandler (void);


void tim3_init(uint16_t tim_psk, uint16_t tim_arr);
void tim2_init(uint16_t tim_psk, uint16_t tim_arr);
void tim1_init(uint16_t tim_psk, uint16_t tim_arr);
void delay_ms(uint16_t ms);
#endif
