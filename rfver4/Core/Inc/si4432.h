#ifndef SI4432_H
#define SI4432_H
/*		pinout
 * 					()GND
 * 					()SDN  shutdown
 * 					()nIRQ
 * 					()nSEL
 * 					()SCLK
 * 		chip up 	()SDI
 * 					()SDO
 * 					()VDD
 * 					()GPIO2
 * 					()GPIO1
 * () gnd			()GPIO0
 * () rf			()GND
 */
#include "directive.h"
#include "spi.h"
#include "si4432_reg.h"
#include "tim.h"
/*
 *
 *
 *
 *
 *
 */

#define SI4432_RX_TIME_MS 2000
#define SI4432_TX_TIME_MS 1000


#define SI4432_GPIO 			GPIOA
#define Pin_SI4432_MISO 		SPI1_PIN_MISO
#define Pin_SI4432_MOSI 		SPI1_PIN_MOSI
#define Pin_SI4432_nSEL			SPI1_PIN_NSS
#define Pin_SI4432_SCK 			SPI1_PIN_SCK
#define Pin_SI4432_SDN 			PIN_1 //отключение питания(инверсия питания)
#define Pin_SI4432_nIRQ 		PIN_0 //вход прерываний от трансивера

#define SI4432_REG0x7_TX_ON (1<<3)
#define SI4432_REG0x7_RX_ON (1<<2)

#define SI4432_CHIP_TYPE (8)
#define SI4432_PACKET_LENGHT (4)
//#define SI4432_RX_TX_FIFO (0x7F) SI4432_REG_FIFO_Access
#define SI4432_BIT_WRITE_nREAD (0x80)
//packet settings
#define PREAMBLE_LENGTH (4) // 4 byte preamble
#define PD_LENGTH (2) // preamble detection threshold in nibbles

#define SI4432_IS_nIRQ()  ((SI4432_GPIO->IDR & (1<<Pin_SI4432_nIRQ)) == 1)
//

// RF stack enumerations
typedef enum _RF_ENUM
{
	SI4432_RF_OK = 0x00, // function response parameters
	SI4432_RF_ERROR_TIMING = 0x01,
	SI4432_RF_ERROR_PARAMETER = 0x02,
	SI4432_RF_PACKET_RECEIVED = 0x03,
	SI4432_RF_RX_FIFO_ALMOST_FULL = 0x04,
	SI4432_RF_NO_PACKET = 0x05,
	SI4432_RF_CRC_ERROR = 0x06,
} SI4432_ENUM;

int16_t si4432_init(void);
void si4432_idle(void);
void si4432_transmit(uint8_t * packet,uint8_t lenght);

void si4432_receive(void);
SI4432_ENUM si4432_packet_received(uint8_t * rx_data);

uint8_t si4432_read_register(uint8_t address);
void si4432_write_register(uint8_t address, uint8_t value);

int16_t si4432_tx_packet(uint8_t *data_tx_ptr);
int16_t si4432_rx_packet(uint8_t *data_tx_ptr, uint16_t time_to_rx);
extern uint8_t si4432_interrupt;

#endif /* INC_SI4432_H_ */
