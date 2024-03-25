#ifndef DMA_H
#define DMA_H

#include "directive.h"
#include "uart.h"
#include "tim.h"
#include "string.h"
void DMA1_Channel4_IRQHandler();///tx
void DMA1_Channel5_IRQHandler();///rx
void DMA1_Channel6_IRQHandler();///rx2
void DMA1_Channel7_IRQHandler();///tx2

#define DMA1_TX_ENABLE() DMA1_Channel4->CCR |= DMA_CCR_EN/*запуск*/
#define DMA1_TX_DISABLE()  DMA1_Channel4->CCR &= ~DMA_CCR_EN/*отключение*/
#define DMA1_RX_ENABLE() DMA1_Channel5->CCR |= DMA_CCR_EN/*запуск*/
#define DMA1_RX_DISABLE() DMA1_Channel5->CCR &= ~DMA_CCR_EN/*отключение*/

#define DMA1_RX2_ENABLE() DMA1_Channel6->CCR |= DMA_CCR_EN/*запуск*/
#define DMA1_RX2_DISABLE() DMA1_Channel6->CCR &= ~DMA_CCR_EN/*отключение*/
#define DMA1_TX2_ENABLE() DMA1_Channel7->CCR |= DMA_CCR_EN/*запуск*/
#define DMA1_TX2_DISABLE()  DMA1_Channel7->CCR &= ~DMA_CCR_EN/*отключение*/


//от проекта осциллограф
#define DMA_ADC_ON() ADC1->CFGR1 |= ADC_CFGR1_DMAEN/* включение запроса DMA из  АЦП*/
#define DMA_ADC_OFF() ADC1->CFGR1 &= ~(ADC_CFGR1_DMAEN)/* отключение запроса DMA из  АЦП*/
#define DMA_TIM17_ON() TIM17->DIER |= TIM_DIER_UDE/* включение запроса DMA из таймера 17*/
#define DMA_TIM17_OFF() TIM17->DIER &= ~(TIM_DIER_UDE)/* отключение запроса DMA из таймера 17*/
#define DMA_TOGGLE_ADC() {DMA_TIM17_OFF(); DMA_ADC_ON();}/*DMA переключить с тим17 на АЦП */
#define DMA_TOGGLE_TIM17() {DMA_ADC_OFF(); DMA_TIM17_ON();}/*DMA переключить с АЦП на тим17 */

void dma_rx_reinit(uint8_t uart_num,uint16_t rx_volume, char  *rx_pointer);
void dma_tx_reinit(uint8_t uart_num,uint16_t tx_volume, char *tx_pointer);
int16_t dma_init(uint8_t uart_num, uint16_t tx_volume, \
		uint8_t *tx_pointer, uint16_t rx_volume, uint8_t *rx_pointer);

#endif
