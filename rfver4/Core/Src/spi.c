#ifndef SPI_C
#define SPI_C

#include "spi.h"
 uint8_t *global_spi_data_rx;
 uint8_t *global_spi_data_tx;
 uint8_t global_spi_iterator_rx;
 uint8_t global_spi_iterator_tx;

void spi1_init(){
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;//тактирование альтернативных функий

	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;//тактирование  spi
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;//тактирование портов A
	//
	gpio_x_pin_y_config(SPI1_GPIO, SPI1_PIN_NSS, GPIO_CR_MODE_OUTPUT_SPEED_50MHZ, GPIO_CR_CNF_OUTPUT_PUSH_PULL);
	//gpio_x_pin_y_config(SPI1_GPIO, SPI1_PIN_NSS_REQUIRED, GPIO_CR_MODE_OUTPUT_SPEED_50MHZ, GPIO_CR_CNF_OUTPUT_PUSH_PULL);
	//GPIO_PIN_SET(SPI1_GPIO,SPI1_PIN_NSS_REQUIRED);//необходимо чтобы SPI оставался в режиме мастера
	gpio_x_pin_y_config(SPI1_GPIO, SPI1_PIN_MISO, GPIO_CR_MODE_INPUT, GPIO_CR_CNF_INPUT_FLOATING);
	gpio_x_pin_y_config(SPI1_GPIO, SPI1_PIN_MOSI, GPIO_CR_MODE_OUTPUT_SPEED_50MHZ, GPIO_CR_CNF_ALTERNATE_FUNCTION_PUSH_PULL);
	gpio_x_pin_y_config(SPI1_GPIO, SPI1_PIN_SCK, GPIO_CR_MODE_OUTPUT_SPEED_50MHZ, GPIO_CR_CNF_ALTERNATE_FUNCTION_PUSH_PULL);
	/*
	 * 2-line unidirectional data mode selected
	 * CRC calculation disabled
	 * 8-bit data frame format is selected for transmission/reception
	 * Software slave management disabled
	 * MSB transmitted first
	 * fPCLK/8
	 * Master configuration
	 * CK to 0 when idle
	 * The first clock transition is the first data capture edge
	 */
	SPI1->CR1 = 0;

	SPI1->CR1 |= SPI_CR1_MSTR;//Master configuration
	SPI1->CR1 |= SPI_CR1_BR_1 | SPI_CR1_BR_0;//010: fPCLK/16
	//SPI1->CR1 |= SPI_CR1_SSM;//программное управление NSS игнорирование значения NSS оно берется из SSI
	//SPI1->CR1 |= SPI_CR1_SSI;//игнорирование значения NSS

	SPI1->CR2 = 0;
	SPI1->CR2 = SPI_CR2_SSOE;// Выход SS включен в режиме master (ведущий), при этом нет возможности работать в multimaster режиме
	//SPI1->CR2 |= SPI_CR2_RXNEIE;// RX buffer not empty interrupt enable
	//SPI1->CR2 |= SPI_CR2_TXEIE;// RX buffer not empty interrupt enable
	SPI1->CR1 |= SPI_CR1_SPE;// Peripheral enabled
	GPIO_PIN_RESET(SPI1_GPIO, SPI1_PIN_NSS);
}

//-----------------------------------------------------------------------------
/// Запись и Чтение данных
//-----------------------------------------------------------------------------
uint8_t spi_basic_byte_send(uint8_t data){
	uint8_t rx_data = 0;
	while (!(SPI1->SR & SPI_SR_TXE)){};//пока не будет готов к передаче
	SPI1->DR = data;
	while((SPI1->SR & SPI_SR_BSY)){}//ждем пока освободится

	while (!(SPI1->SR & SPI_SR_RXNE)){};//когда будет принят байт
	rx_data = SPI1->DR;

	return rx_data;
}

uint8_t spi_data_send_not_use(uint8_t *data_ptr, uint8_t data_length){
	if(!(data_ptr && data_length) ) return SPI_PTR_ERROR;
	global_spi_data_tx = data_ptr;
	global_spi_iterator_tx = data_length;
	SPI1->CR2 |= SPI_CR2_TXEIE;// RX buffer not empty interrupt enable

	return 0;
}

void SPI1_IRQHandler(void){
	/*if(SPI1->SR & SPI_SR_RXNE){
		global_spi_data_rx[global_spi_iterator_rx] = SPI1->DR;
		global_spi_iterator_rx++;
	}
	if(SPI1->SR & SPI_SR_TXE && global_spi_iterator_tx){
		global_spi_iterator_tx--;
		 SPI1->DR = global_spi_data_tx[global_spi_iterator_tx];
	}*/
}
#endif


