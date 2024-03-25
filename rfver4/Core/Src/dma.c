#ifndef DMA_C
#define DMA_C

#include "dma.h"

void dma_rx_reinit(uint8_t uart_num,uint16_t rx_volume, char *rx_pointer){
	switch (uart_num){
	case UART1_NUMBER:
		UART1_RX_CHANEL->CMAR = (uint32_t)rx_pointer;//вносим адрес массива куда писать значения
		UART1_RX_CHANEL->CNDTR = rx_volume;//количество передач
		//UART_RX_CHANEL->CCR |= DMA_CCR_EN;//запуск
		break;
	case UART2_NUMBER:
		UART2_RX_CHANEL->CMAR = (uint32_t)rx_pointer;//вносим адрес массива куда писать значения
		UART2_RX_CHANEL->CNDTR = rx_volume;//количество передач
		//UART2_RX_CHANEL->CCR |= DMA_CCR_EN;//запуск
		break;
	}
}
void dma_tx_reinit(uint8_t uart_num,uint16_t tx_volume, char *tx_pointer){
	switch (uart_num){
	case UART1_NUMBER:
		UART1_TX_CHANEL->CMAR = (uint32_t)tx_pointer;//вносим адрес массива куда писать значения
		UART1_TX_CHANEL->CNDTR = tx_volume;//количество передач
		//UART_TX_CHANEL->CCR |= DMA_CCR_EN;//запуск
		break;
	case UART2_NUMBER:
		UART2_TX_CHANEL->CMAR = (uint32_t)tx_pointer;//вносим адрес массива куда писать значения
		UART2_TX_CHANEL->CNDTR = tx_volume;//количество передач
		//UART2_TX_CHANEL->CCR |= DMA_CCR_EN;//запуск
		break;
	}
}

int16_t dma_init(uint8_t uart_num, uint16_t tx_volume, \
		uint8_t *tx_pointer, uint16_t rx_volume, uint8_t *rx_pointer){
	RCC->AHBENR |= RCC_AHBENR_DMA1EN;//включаем  тактирование DMA
	if(!(tx_pointer && rx_pointer) )
			return ERROR_WRONG_ADDRESS;
	switch (uart_num){
	case UART1_NUMBER://**********uart1**********//
		//из UART в память
		//канал 5 работает на прием UART1
		UART1_RX_CHANEL->CPAR = (uint32_t)&(USART1->DR);//адрес перифирии UART1
		UART1_RX_CHANEL->CMAR = (uint32_t)rx_pointer;//вносим адрес массива куда писать значения
		UART1_RX_CHANEL->CNDTR = rx_volume;//количество передач
		//из перифирии 8бит в память 8бит низкий приоритет
		//адрес памяти инкрементируется, адрес перифирии нет, не цикличный мод
		UART1_RX_CHANEL->CCR =  DMA_CCR_MINC;// | DMA_CCR_CIRC;
		UART1_RX_CHANEL->CCR |= DMA_CCR_TEIE | DMA_CCR_TCIE;//прерывания по ошибке транзакции и по окончанию
		//UART_RX_CHANEL->CCR |= DMA_CCR_EN;//запуск
		//канал 4 работает на передачу UART1
		UART1_TX_CHANEL->CPAR = (uint32_t)&(USART1->DR);//адрес перифирии UART1
		UART1_TX_CHANEL->CMAR = (uint32_t)tx_pointer;//вносим адрес массива откуда брать значения
		UART1_TX_CHANEL->CNDTR = tx_volume;//количество передач
		//из памяти 8бит в перифирию 8бит низкий приоритет
		//адрес памяти инкрементируется, адрес перифирии нет, не цикличный мод
		UART1_TX_CHANEL->CCR =  DMA_CCR_MINC | DMA_CCR_DIR;// | DMA_CCR_CIRC;
		UART1_TX_CHANEL->CCR |= DMA_CCR_TEIE | DMA_CCR_TCIE;//прерывания по ошибке транзакции и по окончанию
		//UART_TX_CHANEL->CCR |= DMA_CCR_EN;//запуск
		break;
	case UART2_NUMBER://**********uart2**********//
		//из UART в память
		//канал 6 работает на прием UART2
		UART2_RX_CHANEL->CPAR = (uint32_t)&(USART2->DR);//адрес перифирии UART1
		UART2_RX_CHANEL->CMAR = (uint32_t)rx_pointer;//вносим адрес массива куда писать значения
		UART2_RX_CHANEL->CNDTR = rx_volume;//количество передач
		//из перифирии 8бит в память 8бит низкий приоритет
		//адрес памяти инкрементируется, адрес перифирии нет, не цикличный мод
		UART2_RX_CHANEL->CCR =  DMA_CCR_MINC;// | DMA_CCR_CIRC;
		UART2_RX_CHANEL->CCR |= DMA_CCR_TEIE | DMA_CCR_TCIE;//прерывания по ошибке транзакции и по окончанию
		//UART2_RX_CHANEL->CCR |= DMA_CCR_EN;//запуск
		//канал 7 работает на передачу UART1
		UART2_TX_CHANEL->CPAR = (uint32_t)&(USART2->DR);//адрес перифирии UART1
		UART2_TX_CHANEL->CMAR = (uint32_t)tx_pointer;//вносим адрес массива откуда брать значения
		UART2_TX_CHANEL->CNDTR = tx_volume;//количество передач
		//из памяти 8бит в перифирию 8бит низкий приоритет
		//адрес памяти инкрементируется, адрес перифирии нет, не цикличный мод
		UART2_TX_CHANEL->CCR =  DMA_CCR_MINC | DMA_CCR_DIR;// | DMA_CCR_CIRC;
		UART2_TX_CHANEL->CCR |= DMA_CCR_TEIE | DMA_CCR_TCIE;//прерывания по ошибке транзакции и по окончанию
		//UART2_TX_CHANEL->CCR |= DMA_CCR_EN;//запуск
		break;
	}
	return 0;
}

void DMA1_Channel4_IRQHandler(){///tx
	if(DMA1->ISR & DMA_ISR_TCIF4){//прерывание по окочанию передачи
		global_uart_flag_data_transmitted = 1;
		DMA1_TX_DISABLE();
	}
	if(DMA1->ISR & DMA_ISR_TEIF4){//прерывание при ошибке передачи
		asm("nop");
	}
	DMA1->IFCR = DMA_IFCR_CGIF4  | DMA_IFCR_CHTIF4 | DMA_IFCR_CTCIF4 | DMA_IFCR_CTEIF4;
}
void DMA1_Channel5_IRQHandler(){///rx
	if(DMA1->ISR & DMA_ISR_TCIF5){//прерывание по окочанию приема
		DMA1_RX_DISABLE();
		global_uart2_rx_time = global_relative_time;
		global_uart_flag_data_recieved = 1;
		global_uart_rx_iterator = UART_RX_BUFF_SIZE - UART1_RX_CHANEL->CNDTR;//буфер минус переданные байты
		global_uart_rx_data[global_uart_rx_iterator] = '\0';
	}
	if(DMA1->ISR & DMA_ISR_TEIF5){
			asm("nop");
		}
	DMA1->IFCR = DMA_IFCR_CGIF5  | DMA_IFCR_CHTIF5 | DMA_IFCR_CTCIF5 | DMA_IFCR_CTEIF5;
}


void DMA1_Channel6_IRQHandler(){///rx
	if(DMA1->ISR & DMA_ISR_TCIF6){//прерывание по окочанию приема
		DMA1_RX2_DISABLE();
		global_uart2_flag_data_recieved = 1;
		global_uart2_rx_iterator = UART_RX_BUFF_SIZE - UART2_RX_CHANEL->CNDTR;//буфер минус переданные байты
		global_uart2_rx_data[global_uart2_rx_iterator] = '\0';
		global_uart2_rx_time = global_relative_time;
	}
	if(DMA1->ISR & DMA_ISR_TEIF6){
			asm("nop");
		}
	DMA1->IFCR = DMA_IFCR_CGIF6  | DMA_IFCR_CHTIF6 | DMA_IFCR_CTCIF6 | DMA_IFCR_CTEIF6;
}

void DMA1_Channel7_IRQHandler(){///tx
	if(DMA1->ISR & DMA_ISR_TCIF7){//прерывание по окочанию передачи
		global_uart2_flag_data_transmitted = 1;
		DMA1_TX2_DISABLE();
	}
	if(DMA1->ISR & DMA_ISR_TEIF7){//прерывание при ошибке передачи
		asm("nop");
	}
	DMA1->IFCR = DMA_IFCR_CGIF7  | DMA_IFCR_CHTIF7 | DMA_IFCR_CTCIF7 | DMA_IFCR_CTEIF7;
}
#endif
