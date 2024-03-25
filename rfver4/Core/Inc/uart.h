#ifndef UART_H
#define UART_H

#include "directive.h"
#include "dma.h"
#include "string.h"
#include "tim.h"
    //pa11 enable
    //pa12 command enable
    //pa10 rxD
    //pa9 txD
#define UART_RX_BUFF_SIZE 255
#define UART_TX_BUFF_SIZE 255
//rf si4432
#define UART1_CLOCK (36*FREQUENCY_MHZ)
#define UART1_NUMBER 1
#define UART1_PORT GPIOA
#define UART1_TX_PIN PIN_9
#define UART1_RX_PIN PIN_10
#define UART1_BAUD_RATE 9600
#define UART1_RX_CHANEL DMA1_Channel5
#define UART1_TX_CHANEL DMA1_Channel4
#define UART1_GPIO_CLOCK_ON() {RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;}
//gps neo6m
#define UART2_CLOCK (36*FREQUENCY_MHZ)
#define UART2_NUMBER 2
#define UART2_PORT GPIOA
#define UART2_TX_PIN PIN_2
#define UART2_RX_PIN PIN_3
#define UART2_BAUD_RATE 9600
#define UART2_RX_CHANEL DMA1_Channel6
#define UART2_TX_CHANEL DMA1_Channel7
#define UART2_GPIO_CLOCK_ON() {RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;}
#define UART2_CLOCK_ON() {RCC->APB1ENR |= RCC_APB1ENR_USART2EN;}


#define UART_RX_NOT_READY -1

extern char *global_uart_tx_data;
extern char *global_uart_rx_data;

extern int16_t global_uart_tx_iterator;
extern int16_t global_uart_rx_iterator;
extern uint8_t global_uart_flag_data_recieved;
extern uint8_t global_uart_flag_data_transmitted;

//uart2
extern char *global_uart2_tx_data;
extern char *global_uart2_rx_data;
extern int32_t global_uart2_rx_time;
extern int16_t global_uart2_tx_iterator;
extern int16_t global_uart2_rx_iterator;
extern uint8_t global_uart2_flag_data_recieved;
extern uint8_t global_uart2_flag_data_transmitted;

void USART1_IRQHandler();//вызывается по окончанию приема
void USART2_IRQHandler();

int16_t uart1_init(char * str_rx, char * str_tx);
int16_t uart2_init(char * str_rx, char * str_tx);
void uart_echo();
uint8_t uart_transmit_string(uint8_t uart_num, char *str_tx, uint8_t lenght);//возвращает количество успешно переданных символов
uint8_t uart_recieve_string(uint8_t uart_num, char *str_rx);
#endif /* UART_H_ */
