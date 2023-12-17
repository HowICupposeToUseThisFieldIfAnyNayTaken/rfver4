
#include "directive.h"
#include "uart.h"
#include "spi.h"
#include "dma.h"
#include "tim.h"
#include "neo6m.h"

#include "protokol.h"
#include "string.h"
//#include "cc1101.h"
#include "si4432.h"


#define CC1101_RX_BUFFER_SIZE 64
#define NEW_BUFF_SIZE 50

uint32_t global_relative_time = 0;
uint32_t global_time_is_real = 0;

void gpio_x_pin_y_config(GPIO_TypeDef * GPIOx,uint8_t pin_y, uint8_t mode, uint8_t cnf );
void gpio_init();
void rcc_init();
void peripherial_init();
void interrupts_first_init();
void exti0_init();
void exti1_init();
void echo_general(char * str_recieve,char * data_to_transfer);

uint8_t main_rf_slave_echo(uint8_t *rf_rx_data);
uint8_t main_rf_master_echo(uint8_t *rf_rx_data);
uint8_t main_tx_operation(uint8_t * data_ptr, uint8_t n_tx_data);
int8_t  main_rx_operation(uint8_t * data_arr, uint8_t max_data_to_read);
uint8_t cc1101_wait_until_iddle_state();
void sync_time(uint8_t * str_gps_rx, struct ubx_tim_tp * s_neo6m_msg);

const uint8_t is_master = 1;
const uint8_t use_uart = 1;
const uint8_t use_gps = 0;

//////////////////////global variables

int main(void){
	int i;
	int tmp = 65;
	uint8_t chip_type =0;
	uint8_t n_bytes_recieved =0;
	uint8_t crc_ok = 0;
	uint8_t  rf_rx_data[NEW_BUFF_SIZE];
	//
	const char str_sgnl_error[] = {'n','s','g','n'};
	char data_to_transfer[255];
	char str_ready[]="\nready to transfer\n";
	char str_test[] = "AT\r\n\0";
	char str_recieve[UART_RX_BUFF_SIZE];
	char str_transmit_buffer[UART_TX_BUFF_SIZE] = "no1\n\0";
	char str_gps_rx[UART_RX_BUFF_SIZE] = "start data";
	char str_gps_tx[UART_TX_BUFF_SIZE] = "no1\n\0";
	struct ubx_tim_tp s_neo6m_msg;
	//general init
	n_bytes_recieved = 0;
	for(i = 0; i< NEW_BUFF_SIZE;i++){
		 rf_rx_data[i] = 0;
	}
	peripherial_init();
	{
		int16_t err_code = 0;
		if(is_master && use_uart)
			uart1_init(str_recieve,str_transmit_buffer );
		if(use_gps)
			uart2_init(str_gps_rx,str_gps_tx );
		if(is_master && use_uart)
			dma_init(UART1_NUMBER, 5, global_uart_tx_data, UART_RX_BUFF_SIZE, global_uart_rx_data);
		if(use_gps)
			dma_init(UART2_NUMBER, 5, global_uart2_tx_data, UART_RX_BUFF_SIZE, global_uart2_rx_data);

		do{
			err_code = si4432_init();
		}while(err_code != ERROR_NO_ERROR);
	}
	interrupts_first_init();
	if(is_master && use_uart)
			BLUETOOTH_ENABLE();


	//end of general init

	do{//если модуль не отвечает, проблемы с пайкой, программа будет  висеть здесь
		chip_type = si4432_read_register(SI4432_REG_Device_Type);
	}while(chip_type !=  SI4432_CHIP_TYPE);
	//сообщаем о готовности
	if(is_master && use_uart){
		i = str_len(str_ready);
		uart_transmit_string(UART1_NUMBER, str_ready, str_len(str_ready));
	}
	while(1){
		if(use_gps){
			sync_time(str_gps_rx, &s_neo6m_msg);
		}
		echo_general(str_recieve, data_to_transfer);

	}
}
/////////////////////////////////////////////////////////////////////////////////
void sync_time(uint8_t * str_gps_rx, struct ubx_tim_tp * s_neo6m_msg){
	static uint8_t waiting_tim_tp = 0;

	if(!global_time_is_real){//время не синхронизированно
		if(!waiting_tim_tp){//включаем прием пока не придет сообщение
					uart_recieve_string(UART2_NUMBER, str_gps_rx);
					waiting_tim_tp = 1;
		}
		if(global_uart2_flag_data_recieved){//если пришло сообщение на юарт
			if(global_relative_time){//если первый тайм пульс пришел и пошел отсчет времени
				int16_t is_correct = ubx_msg_is_correct(str_gps_rx);//если формат сообщения ubx и сообщение tim-tp
				if(is_correct > 0) {
					ubx_tim_tp_decrypt(str_gps_rx, s_neo6m_msg);
					ubx_set_time(s_neo6m_msg);
				}
			}
			else
				waiting_tim_tp = 0;
		}
	}

}

void peripherial_init(){
	rcc_init();
	tim1_init(TIM1_MSEC_PSK,1000);//1000ms
	exti0_init();
	exti1_init();
	gpio_init();
	spi1_init();

	//cc1101_init();
}

void interrupts_first_init(){
	NVIC_EnableIRQ(USART1_IRQn);
	NVIC_EnableIRQ(USART2_IRQn);
	if(is_master){
		NVIC_EnableIRQ(DMA1_Channel4_IRQn);
		NVIC_EnableIRQ(DMA1_Channel5_IRQn);
	}
	if(use_gps){
			NVIC_EnableIRQ(DMA1_Channel6_IRQn);
			NVIC_EnableIRQ(DMA1_Channel7_IRQn);
		}
	NVIC_EnableIRQ(EXTI0_IRQn);
	NVIC_EnableIRQ(NVIC_NEO6M_PPS_DETECT);
	NVIC_EnableIRQ(TIM3_IRQn);
	 __enable_irq (); // Разрешить прерывания IRQ
	 //__disable_irq ();// Запретить прерывания IRQ
}

void rcc_init(){//36mhz
	//RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;// делим частоту на 2 у медленной шины APB1 до 36мгц
	RCC->CFGR |= RCC_CFGR_PLLMULL9;//умножение частоты на 9
	RCC->CFGR |= RCC_CFGR_PLLXTPRE_HSE_DIV2;//делим входную частоту на 2 от внешнего кварца
	RCC->CFGR |= RCC_CFGR_PLLSRC;//тактирование множителя частоты от внешнего кварца

	RCC->CR |= RCC_CR_HSEON;//включаем внешний кварц
	while(!(RCC->CR & RCC_CR_HSERDY)){}//пока не готов внешний кварц

	RCC->CR |= RCC_CR_PLLON;//включаем множитель частоты
	while(!(RCC->CR & RCC_CR_PLLRDY)){}//пока не готов множитель частоты

	RCC->CFGR |= RCC_CFGR_SW_PLL;//переключаем SYSCLK на тактирование от PLL
	while(!(RCC->CFGR & RCC_CFGR_SWS_PLL)){}//пока не переключится SYSCLK на PLL
}
void gpio_init(){
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;

	//gpio_x_pin_y_config(GPIOA, BLUETOOTH_PIN_nENABLE, GPIO_CR_MODE_OUTPUT_SPEED_50MHZ, GPIO_CR_CNF_OUTPUT_OPEN_DRAIN);
	//gpio_x_pin_y_config(GPIOA, BLUETOOTH_PIN_AT_COMMANT, GPIO_CR_MODE_OUTPUT_SPEED_50MHZ, GPIO_CR_CNF_OUTPUT_OPEN_DRAIN);
	//gpio_x_pin_y_config(GPIOB, 1, GPIO_CR_MODE_OUTPUT_SPEED_50MHZ, GPIO_CR_CNF_OUTPUT_OPEN_DRAIN);
	//gpio_x_pin_y_config(GPIOB, 2, GPIO_CR_MODE_OUTPUT_SPEED_50MHZ, GPIO_CR_CNF_OUTPUT_OPEN_DRAIN);
}



void gpio_x_pin_y_config(GPIO_TypeDef * GPIOx,uint8_t pin_y, uint8_t mode, uint8_t cnf ){
	if(pin_y > 7){//другой регистр
		pin_y = pin_y-8;
		GPIOx->CRH = (GPIOx->CRH & 	( ~((uint32_t)(0x0003<<(pin_y*4))))) | mode<<(pin_y*4);
		GPIOx->CRH = (GPIOx->CRH & 	( ~((uint32_t)(0x0003<<(2+pin_y*4))))) | cnf<<(2+pin_y*4);

	}
	else{
		GPIOx->CRL = (GPIOx->CRL & 	( ~((uint32_t)(0x0003<<(pin_y*4))))) | mode<<(pin_y*4);
		GPIOx->CRL = (GPIOx->CRL & 	( ~((uint32_t)(0x0003<<(2+pin_y*4))))) | cnf<<(2+pin_y*4);
	}
}

void exti0_init(){
	//exti0 - a0 falling
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;//тактирование альтернативных функий
	EXTI->IMR |= EXTI_IMR_IM0;//прерывание по 0му пину
	EXTI->FTSR |= EXTI_FTSR_FT0;//falling trigger event configuration bit of line x
	//EXTI->RTSR |= EXTI_RTSR_RT0;//rising trigger event configuration bit of line x
	AFIO->EXTICR[0] = AFIO_EXTICR1_EXTI0_PA;//прерывания искать по порту А
}
void exti1_init(){
	//exti1 - b1 rising
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;//тактирование альтернативных функий
	EXTI->IMR |= EXTI_IMR_IM1;//прерывание по 1му пину
	//EXTI->FTSR |= EXTI_FTSR_FT0;//falling trigger event configuration bit of line x
	EXTI->RTSR |= EXTI_RTSR_RT1;//rising trigger event configuration bit of line x
	AFIO->EXTICR[0] = Neo6M_EXTI;//прерывания искать по порту B
}

void EXTI0_IRQHandler(){
	EXTI->PR |= EXTI_PR_PR0;
	si4432_interrupt = 1;
}
void EXTI1_IRQHandler(){
	EXTI->PR |= EXTI_PR_PR1;
	TIM1_ENABLE();
	NVIC_DisableIRQ(NVIC_NEO6M_PPS_DETECT);
	neo6m_interrupt = 1;
}
void delay_ms(uint16_t msec){
	const uint16_t psk = CPU_CLOCK_MHZ * FREQUENCY_KHZ;
	//tim2_init(psk, msec);
	//TIM2_ENABLE();

	//while(!global_main_flag_tim2_update){};
	for(int i = 0; i < msec *CPU_CLOCK_MHZ*50; i++){
		//иначе работает криво
	}
}

//-----------------------------------------------------------------------------
/// Эхо юарт-si4432 - si4432(slave) - si4432 - юарт
//-----------------------------------------------------------------------------
void echo_general(char * str_recieve,char * data_to_transfer){
	//общее эхо
	uint8_t rx_len = 0;
	uint16_t i = 0;
	uint8_t is_data_in_stock = 0;
	while(is_master){//для ведущего устройства
		//
		if(use_uart && (!is_data_in_stock)){
			uart_recieve_string(UART1_NUMBER, str_recieve);
			//waiting data via uart
			while(global_uart_flag_data_recieved <= 0){};
			is_data_in_stock = 1;
		}
		tim2_time_ms(SI4432_TX_TIME_MS);
		//1sec tx
		do{
			si4432_tx_packet(str_recieve);
		}
		while(!global_main_flag_tim2_update);
		//2sec recieve data
		rx_len = 0;
		rx_len = si4432_rx_packet(data_to_transfer, SI4432_RX_TIME_MS);
		if(rx_len > 0){
			//data is sucsesful received

			if(use_uart){
				char str = '\n';
				uart_transmit_string(UART1_NUMBER, data_to_transfer, rx_len);
				//uart_transmit_string(&str, 1);
			}
			rx_len = 0;
			is_data_in_stock =0;
			return;
		}
	}//общее эхо конец
	while(is_master == 0){//для ведомого устройства

		for(i =0; i< SI4432_PACKET_LENGHT; i++){
			data_to_transfer[i] = 0;
		}
		rx_len = 0;
		rx_len = si4432_rx_packet(data_to_transfer, SI4432_RX_TIME_MS);

		if(rx_len > 0){
			char str[] = "!tim";
			//data_to_transfer[3] = 'o';
			if(!str_compare(data_to_transfer, str, 4)){
				data_to_transfer[3] = global_relative_time&0xff;
				data_to_transfer[2] = (global_relative_time>>8)&0xff;
				data_to_transfer[1] = (global_relative_time>>16)&0xff;
				data_to_transfer[0] = (global_relative_time>>24)&0xff;
			}
			si4432_tx_packet(data_to_transfer);
			return;
		}
	}//общее эхо конец

}
/*
 * struct date_format{
	uint16_t year;
	uint8_t	month;
	uint8_t	day;
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
	uint16_t sub_seconds;
};
 *
 */
void date_increment(struct date_format * s_date){
	s_date->second++;
	if(s_date->second >= 60){
		s_date->minute +=s_date->second / 60;
		s_date->second %= 60;

		s_date->hour +=s_date->minute / 60;
		s_date->minute %= 60;

		s_date->day +=s_date->hour / 24;
		s_date->hour %= 24;
	}
}
