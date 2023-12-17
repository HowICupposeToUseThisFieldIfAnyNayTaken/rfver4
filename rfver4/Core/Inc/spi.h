#ifndef SPI_H
#define SPI_H

#include "directive.h"
#define SPI1_GPIO 			GPIOA
#define SPI1_PIN_NSS  		PIN_4
#define SPI1_PIN_SCK  		PIN_5
#define SPI1_PIN_MISO 		PIN_6
#define SPI1_PIN_MOSI 		PIN_7

#define SPI1_NSS_LOW() 		{GPIO_PIN_RESET(SPI1_GPIO,SPI1_PIN_NSS);}
#define SPI1_NSS_HIGHT() 	{GPIO_PIN_SET(SPI1_GPIO,SPI1_PIN_NSS);}
#define SPI1_SCK_LOW() 		{GPIO_PIN_RESET(SPI1_GPIO,SPI1_PIN_SCK);}
#define SPI1_SCK_HIGHT() 	{GPIO_PIN_SET(SPI1_GPIO,SPI1_PIN_SCK);}
#define SPI1_MISO_LOW() 	{GPIO_PIN_RESET(SPI1_GPIO,SPI1_PIN_MISO);}
#define SPI1_MISO_HIGHT() 	{GPIO_PIN_SET(SPI1_GPIO,SPI1_PIN_MISO);}
#define SPI1_MOSI_LOW() 	{GPIO_PIN_RESET(SPI1_GPIO,SPI1_PIN_MOSI);}
#define SPI1_MOSI_HIGHT() 	{GPIO_PIN_SET(SPI1_GPIO,SPI1_PIN_MOSI);}

#define SPI_PTR_ERROR 1

extern uint8_t *global_spi_data_rx;
extern uint8_t *global_spi_data_tx;
extern uint8_t global_spi_iterator_rx;
extern uint8_t global_spi_iterator_tx;

void SPI1_IRQHandler(void);

void spi1_init();
uint8_t spi_data_send(uint8_t *data_ptr, uint8_t data_length);
uint8_t spi_basic_byte_send(uint8_t data);

#endif
