#ifndef TIM_C
#define TIM_C

#include "tim.h"

uint8_t global_main_flag_tim3_update = 0;
uint8_t global_main_flag_tim2_update = 0;
uint8_t global_main_flag_tim1_update = 0;
void tim2_time_ms(uint16_t msec){
	const uint16_t psk = CPU_CLOCK_MHZ * FREQUENCY_KHZ;
	tim2_init(psk, msec);
	TIM2_ENABLE();
}
void tim3_init(uint16_t tim_psk, uint16_t tim_arr){
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN; //тактирование таймера3
	 TIM3->PSC = tim_psk - 1; /* предделитель */
	 TIM3->ARR = tim_arr - 1; /* значение до которого ведется счет */
	 TIM3->DIER |= TIM_DIER_UIE;//включение прерывания по обновлению
	 TIM3->CR1 |= TIM_CR1_ARPE;
	 TIM3->CNT = 0;
	 global_main_flag_tim3_update = 0;
	 //TIM3->DIER |= TIM_DIER_UDE;//включение DMA по событию переполнения
	 //TIM3->CR1 |= TIM_CR1_CEN; /* включение*/
	 //NVIC_EnableIRQ(TIM3_IRQn); /* (9) */
}

void TIM3_IRQHandler (void) {
  TIM3->SR &= ~TIM_SR_UIF;
  global_main_flag_tim3_update=1;
  if(GPIOC->ODR & 1<<8){
 	 //GPIOC->BSRR = GPIO_BSRR_BR_8;

 	 //GPIOC->ODR &= ~(1<<6);
  }
  else
  {
 	 //GPIOC->ODR |= (1<<6);
  //GPIOC->BSRR = GPIO_BSRR_BS_8;
  }
}

void TIM2_IRQHandler (void) {
  TIM2->SR &= ~TIM_SR_UIF;
  global_main_flag_tim2_update=1;
}

void tim2_init(uint16_t tim_psk, uint16_t tim_arr){
	RCC -> APB1ENR |= RCC_APB1ENR_TIM2EN;//тактирование таймера2
	//TIM2->CR1 = TIM_CR1_URS;//прерывания по событию переполнения
	TIM2->DIER = TIM_DIER_UIE;//включение прерывания по обновлению
	TIM2->SMCR = 0;

	 //TIMx_ARR и TIMx_CCRx
	TIM2->CR1 |= TIM_CR1_ARPE;
	 TIM2->PSC = tim_psk -1;
	 TIM2->ARR = tim_arr -1;
	 TIM2->CNT 	= 0;
	 NVIC_EnableIRQ (TIM2_IRQn);
	//TIM2->CR1 |= TIM_CR1_CEN;
	 global_main_flag_tim2_update = 0;
}

void TIM1_UP_IRQHandler (void) {
  TIM1->SR &= ~TIM_SR_UIF;
  global_main_flag_tim1_update=1;
  global_relative_time++;
}

void tim1_init(uint16_t tim_psk, uint16_t tim_arr){
	RCC -> APB2ENR |= RCC_APB2ENR_TIM1EN;//тактирование таймера1
	TIM2->CR1 = TIM_CR1_URS;// Only counter overflow/underflow generates an update interrupt
	TIM1->DIER = TIM_DIER_UIE;//включение прерывания по обновлению
	TIM1->SMCR = 0;//slave mode control register

	 //TIMx_ARR и TIMx_CCRx
	TIM1->CR1 |= TIM_CR1_ARPE;
	 TIM1->PSC = tim_psk -1;
	 TIM1->ARR = tim_arr -1;
	 TIM1->CNT 	= 0;
	 NVIC_EnableIRQ (TIM1_UP_IRQn);
	//TIM2->CR1 |= TIM_CR1_CEN;
	 global_main_flag_tim1_update = 0;
}

#endif
