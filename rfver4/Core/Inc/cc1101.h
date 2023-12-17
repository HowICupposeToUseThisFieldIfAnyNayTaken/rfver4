#ifndef CC1101_H
#define CC1101_H

/*
 *  gdo0 	a0 pin3
 *  gdo2 	a1 pin8
 *  nCS 	a2 pin4
 *  sck 	a5 pin5
 *  miso  	a6 pin 7 <-
 *  mosi 	a7 pin6  ->

*/
/*
 * status byte
 * X7			x6x5x4		x3x2x1x0
 * CHIP_RDYn	STATE[2:0]	FIFO_BYTES_AVAILABLE[3:0]
 * 				000  iddle
 * 				001	 RX
 * 				010  TX
 * 				011  FAST TX
 * 				100 CALIBRATE
 * 				101 SETTLING PLL is settling
 * 				110 RXFIFO_OVERFLOW
 * 				111 TXFIFO_UNDERFLOW
 */
#include "directive.h"
#include "spi.h"
#include "cc1101_reg.h"
#define CC1101_BURST_ACCESS_BIT (1<<6)
#define CC1101_READ_nWRITE_BIT (1<<7)

#define CC1101_GPIO GPIOA
#define CC1101_GDO_2_PIN PIN_0


#define CC1101_STROBE_ADDRES_MIN 0x30
#define CC1101_STROBE_ADDRES_MAX 0x3D
#define CC1101_STROBE_ADRESS_ERROR -1

#define CC1101_STATUS_CHIP_nRDY_OFFSET 			7
#define CC1101_STATUS_CHIP_nRDY					1<<CC1101_STATUS_CHIP_nRDY_OFFSET
#define CC1101_STATUS_STATE_OFFET 				4
#define CC1101_STATUS_STATE_IDDLE				0<<CC1101_STATUS_STATE_OFFET/*IDLE state(Also reported for some transitional states instead of SETTLING or CALIBRATE)*/
#define CC1101_STATUS_STATE_RX					1<<CC1101_STATUS_STATE_OFFET/*Receive mode*/
#define CC1101_STATUS_STATE_TX					2<<CC1101_STATUS_STATE_OFFET/*Transmit mode*/
#define CC1101_STATUS_STATE_FSTXON				3<<CC1101_STATUS_STATE_OFFET/*Fast TX ready*/
#define CC1101_STATUS_STATE_CALIBRATE			4<<CC1101_STATUS_STATE_OFFET/*Frequency synthesizer calibration is running*/
#define CC1101_STATUS_STATE_SETTLING			5<<CC1101_STATUS_STATE_OFFET/*RX FIFO has overflowed. Read out any useful data, then flush the FIFO with SFRX*/
#define CC1101_STATUS_STATE_RXFIFO_OVERFLOW		6<<CC1101_STATUS_STATE_OFFET/*PLL is settling*/
#define CC1101_STATUS_STATE_TXFIFO_UNDERFLOW	7<<CC1101_STATUS_STATE_OFFET/* TX FIFO has underflowed. Acknowledge with SFTX*/
#define CC1101_STATUS_STATE_MASK 				7<<CC1101_STATUS_STATE_OFFET
#define CC1101_STATUS_FIFO_BYTES_AVALIABLE_MASK 3

#define CC1101_WRITE_SINGLE   	0x00
#define CC1101_WRITE_BURST		(CC1101_BURST_ACCESS_BIT)
#define CC1101_READ_SINGLE   	(CC1101_READ_NWRITE_BIT)
#define CC1101_READ_BURST     	(CC1101_READ_NWRITE_BIT | CC1101_BURST_ACCESS_BIT)

#define 	CC1101_RXBYTES			0xFB
#define 	CC1101_TXBYTES			0xFA
#define 	CC1101_VERSION			0xF1
#define 	CC1101_PATABLE      	0x3E

#define 	CC1101_RSSI_BYTE_OFFSET 	1
#define 	CC1101_LQI_CRC_BYTE_OFFSET 	2
#define 	CC1101_CRC_OK				0x80

#define  	CC1101_PA_POWER_NEG30		0x03/*-30DB*/
#define  	CC1101_PA_POWER_NEG20		0x17/*-20DB*/
#define  	CC1101_PA_POWER_NEG15		0x1D/*-15DB*/
#define  	CC1101_PA_POWER_NEG10		0x26/*-10DB*/
#define  	CC1101_PA_POWER_NEG6		0x37/* -6DB*/
#define  	CC1101_PA_POWER_0			0x50/*  0DB*/
#define  	CC1101_PA_POWER_POS_5		0x86/*  5DB*/
#define  	CC1101_PA_POWER_POS_7		0xCD/*  7DB*/
#define  	CC1101_PA_POWER_POS_10		0xC5/* 10DB*/

#define  	CC1101_FIXED_PACKET_LENGTH 4
#define  	CC1101_ADDRESS 27
#define 	CC1101_RSSI_OFFSET 74

#define		CC1101_TX_FIFO_SINGL	0x3F// Single byte access to TX FIFO
#define		CC1101_TX_FIFO_BURST	0x7F// Burst access to TX FIFO
#define		CC1101_RX_FIFO_SINGL	0xBF// Single byte access to RX FIFO
#define		CC1101_RX_FIFO_BURST	0xFF// Burst access to RX FIFO

#define  	CC1101_TX_TO_GUARANTEED_RX 10
extern uint8_t cc1101_register_config[];
extern uint8_t cc1101_packet_processing_ended;

void cc1101_init(void);
uint8_t cc1101_data_read(uint8_t reg_addres);
uint8_t cc1101_data_send(uint8_t reg_addres,uint8_t tx_data);
uint8_t cc1101_data_read_burst(uint8_t reg_addres, uint8_t * data_ptr,uint8_t burst_size);//возвращает бат статуса
uint8_t cc1101_data_write_burst(uint8_t reg_addres, uint8_t * data_ptr,uint8_t burst_size);
int16_t cc1101_command_strobe(uint8_t comand, uint8_t read_nwrite_bit);
uint8_t cc1101_tx_send_single_byte(uint8_t byte);
uint8_t cc1101_rx_read_single_byte();
int16_t cc1101_rssi_convert_to_dbm(uint8_t rssi);
uint8_t cc1101_wait_until_iddle_state();
uint8_t cc1101_paket_crc_check(uint8_t * packet_ptr);
#endif
/*
 * The following header bytes access the FIFOs:
• 0x3F: Single byte access to TX FIFO
• 0x7F: Burst access to TX FIFO
• 0xBF: Single byte access to RX FIFO
• 0xFF: Burst access to RX FIFO
 */
