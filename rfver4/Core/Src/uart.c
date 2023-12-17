#ifndef UART_C
#define UART_C

#include "uart.h"
//uart
char *global_uart_tx_data;
char *global_uart_rx_data;
int16_t global_uart_tx_iterator = -1;
int16_t global_uart_rx_iterator = -1;
uint8_t global_uart_flag_data_recieved = 0;
uint8_t global_uart_flag_data_transmitted = 0;

//uart2
char *global_uart2_tx_data;
char *global_uart2_rx_data;
int32_t global_uart2_rx_time = -1;
int16_t global_uart2_tx_iterator = -1;
int16_t global_uart2_rx_iterator = -1;
uint8_t global_uart2_flag_data_recieved = 0;
uint8_t global_uart2_flag_data_transmitted = 0;


int16_t uart1_init(char * str_rx, char * str_tx){
	if(!(str_rx || str_tx) ) return ERROR_WRONG_ADDRESS;
	global_uart_rx_data = str_rx;
	global_uart_tx_data = str_tx;
	UART1_GPIO_CLOCK_ON();//тактирование портов uart
	gpio_x_pin_y_config(UART1_PORT, UART1_TX_PIN, GPIO_CR_MODE_OUTPUT_SPEED_50MHZ, GPIO_CR_CNF_ALTERNATE_FUNCTION_PUSH_PULL);//pa9 tx
	gpio_x_pin_y_config(UART1_PORT, UART1_RX_PIN, GPIO_CR_MODE_INPUT, GPIO_CR_CNF_INPUT_FLOATING);//pa10 rx
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;//тактирование usart
	USART1->CR1 |= USART_CR1_UE;//включение usart
	//8bit word lenght
	//1stop bit
	//dma
	//считаем baudrate не по даташиту но результат такой же
	USART1->BRR = UART1_CLOCK / UART1_BAUD_RATE;//https://easystm32.ru/interfaces/15-uart-in-stm32-part-1/
	//USART1->CR1 |= USART_CR1_M
	//USART1->CR1 |= USART_CR1_TXEIE;// прерывание по передаче
	//USART1->CR1 |= USART_CR1_RXNEIE;// прерывание по приему ORE=1 or RXNE=1 in the USART_SR register
	USART1->CR1 |=  USART_CR1_RE | USART_CR1_TE;//разрешение приема и передачи
	USART1->CR1 |=  USART_CR1_IDLEIE;//прерывание при прекращении пересылки данных по Rx
	//USART1->CR2 |=  USART_CR2_LINEN;//включение поиска конца кадра(break последовательности)
	//USART1->CR2 |=  USART_CR2_LBDIE;//прерывание по break последовательности
	USART1->CR3 |= USART_CR3_DMAR | USART_CR3_DMAT; //прием и передача через DMA
	return 0;
}

int16_t uart2_init(char * str_rx, char * str_tx){
	if(!(str_rx || str_tx) ) return ERROR_WRONG_ADDRESS;
	global_uart2_rx_data = str_rx;
	global_uart2_tx_data = str_tx;
	UART2_GPIO_CLOCK_ON();//тактирование портов uart2
	gpio_x_pin_y_config(UART2_PORT, UART2_TX_PIN, GPIO_CR_MODE_OUTPUT_SPEED_50MHZ, GPIO_CR_CNF_ALTERNATE_FUNCTION_PUSH_PULL);//pa2 tx
	gpio_x_pin_y_config(UART2_PORT, UART2_RX_PIN, GPIO_CR_MODE_INPUT, GPIO_CR_CNF_INPUT_FLOATING);//pa3 rx
	UART2_CLOCK_ON();//тактирование usart2
	USART2->CR1 |= USART_CR1_UE;//включение usart2
	//8bit word lenght 1stop bit dma
	//считаем baudrate не по даташиту но результат такой же
	USART2->BRR = UART2_CLOCK / UART2_BAUD_RATE;//https://easystm32.ru/interfaces/15-uart-in-stm32-part-1/
	USART2->CR1 |=  USART_CR1_RE | USART_CR1_TE;//разрешение приема и передачи
	USART2->CR1 |=  USART_CR1_IDLEIE;//прерывание при прекращении пересылки данных по Rx
	USART2->CR3 |= USART_CR3_DMAR | USART_CR3_DMAT; //прием и передача через DMA
	return 0;
}

uint8_t uart_transmit_string(uint8_t uart_num, char *str_tx, uint8_t lenght){//возвращает количество успешно переданных символов
	if(!str_tx) return ERROR_WRONG_ADDRESS;
	switch (uart_num){
	case UART1_NUMBER:
		global_uart_flag_data_transmitted = 0;
		dma_tx_reinit(uart_num, lenght, str_tx);
		USART1->SR &= (~USART_SR_TC);//сбрасываем бит готовности к передаче
		DMA1_TX_ENABLE();//запуск DMA
		return lenght;
		break;
	case UART2_NUMBER:
		global_uart2_flag_data_transmitted = 0;
		dma_tx_reinit(uart_num, lenght, str_tx);
		USART2->SR &= (~USART_SR_TC);//сбрасываем бит готовности к передаче
		DMA1_TX2_ENABLE();//запуск DMA
		return lenght;
		break;
	}
	return ERROR_WRONG_PARAMETRS;
}

uint8_t uart_recieve_string(uint8_t uart_num, char *str_rx){//возвращает количество успешно принятых символов
	if(!str_rx) return ERROR_WRONG_ADDRESS;
	switch (uart_num){
	case UART1_NUMBER:
		global_uart_flag_data_recieved = 0;
		dma_rx_reinit(uart_num, UART_RX_BUFF_SIZE, str_rx);
		global_uart_rx_iterator = -1;
		global_uart_flag_data_recieved = 0;
		DMA1_RX_ENABLE();//запуск DMA
		break;
	case UART2_NUMBER:
		global_uart2_flag_data_recieved = 0;
		dma_rx_reinit(uart_num, UART_RX_BUFF_SIZE, str_rx);
		global_uart2_rx_iterator = -1;
		global_uart2_flag_data_recieved = 0;
		global_uart2_rx_time = -1;
		DMA1_RX2_ENABLE();//запуск DMA
		break;
	}
	return 0;
}

void uart1_echo(){
	char str_tmp_buffer[256];
	if(global_uart_flag_data_recieved > 0){
	    str_copy(global_uart_rx_data, str_tmp_buffer, global_uart_rx_iterator);
	    if(global_uart_rx_iterator <=0)
	    global_uart_rx_iterator = str_len(str_tmp_buffer);
	    uart_transmit_string(UART1_NUMBER, str_tmp_buffer,global_uart_rx_iterator);
	    uart_recieve_string(UART1_NUMBER, global_uart_rx_data);
	    //GPIOA->BSRR = 1<<(BLUETOOTH_PIN_nENABLE + GPIO_BSRR_BIT_SET);
	    global_uart_flag_data_recieved = 0;
	}
}

void USART1_IRQHandler(){
	char str_reset[] = "\\reset";
	uint32_t clear_iddle_flag = 0;
	int32_t reg_tmp = USART1->SR;//сохраняем статус регистр
	if(reg_tmp &  USART_SR_IDLE){//если на Rx нет движения
		clear_iddle_flag = USART1->SR;
		clear_iddle_flag = USART1->DR;
		clear_iddle_flag = USART1->SR;
		global_uart_flag_data_recieved =  1;
		//global_uart_rx_iterator = -1;
		global_uart_rx_iterator = UART_RX_BUFF_SIZE - UART1_RX_CHANEL->CNDTR;//буфер минус переданные байты

		DMA1_RX_DISABLE();
		if(!str_compare(global_uart_rx_data, str_reset, 6)){
			NVIC_SystemReset();
		}
	}
}

void USART2_IRQHandler(){
	uint32_t clear_iddle_flag = 0;
	int32_t	reg_tmp = USART2->SR;//сохраняем статус регистр
	if(reg_tmp &  USART_SR_IDLE){//если на Rx нет движения
		clear_iddle_flag = USART2->SR;
		clear_iddle_flag = USART2->DR;
		clear_iddle_flag = USART2->SR;
		//clear_iddle_flag = USART2->DR;
		global_uart2_flag_data_recieved =  1;
		//global_uart2_rx_iterator = -1;
		reg_tmp = UART2_RX_CHANEL->CNDTR;
		global_uart2_rx_iterator = UART_RX_BUFF_SIZE - UART2_RX_CHANEL->CNDTR;//буфер минус переданные байты
		global_uart2_rx_time = global_relative_time;
		DMA1_RX2_DISABLE();
	}

}

#endif

