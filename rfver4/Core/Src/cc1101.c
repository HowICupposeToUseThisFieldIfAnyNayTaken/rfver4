#ifndef CC1101_C
#define CC1101_C

#include "cc1101.h"

#define PKTCTRL0_CRC_EN (1<<2)
#define PKTCTRL1_ADR_CHK_NO_BROADCAST (1<<0)
#define PKTCTRL1_APPEND_STATUS (1<<2)
#define PKTCTRL1_CRC_AUTOFLUSH (1<<3)
#define MCSM1_RXOFF_OFSET 2
#define MCSM1_RXOFF_STAY_RX (0x3<<MCSM1_RXOFF_OFSET)

uint8_t cc1101_packet_processing_ended = 0;
uint8_t cc1101_register_config[] = {
	//adr-------value
	IOCFG2  , 0x06,     /*GDO2 Asserts when sync word has been sent / received, and de-asserts at the end of the packet. In RX, the pin will de-assert
							when the optional address check fails or the RX FIFO overflows. In TX the pin will de-assert if the TX FIFO underflows*/
	//IOCFG2  , 0x07,     //GDO2 Asserts when a packet has been received with CRC OK. De-asserts when the first byte is read from the RX FIFO
    FIFOTHR , 0x47,     //RX FIFO and TX FIFO Thresholds
    PKTLEN  , CC1101_FIXED_PACKET_LENGTH,     //Packet Length
	PKTCTRL1, 5,//  | PKTCTRL1_CRC_AUTOFLUSH,
	PKTCTRL0, 4,     //Packet Automation Control
    ADDR    , 0x27,     //Device Address
    FSCTRL1 , 0x06,     //Frequency Synthesizer Control
    FREQ2   , 0x10,     //Frequency Control Word, High Byte
    FREQ1   , 0xA7,     //Frequency Control Word, Middle Byte
    FREQ0   , 0x62,     //Frequency Control Word, Low Byte
    MDMCFG4 , 0xF5,     //Modem Configuration
    MDMCFG3 , 0x83,     //Modem Configuration
    MDMCFG2 , 0x19,     //Modem Configuration
    MDMCFG1 , 0x02,     //Modem Configuration
    DEVIATN , 0x15,     //Modem Deviation Setting
	//MCSM1   , 0x30,//reset value //MCSM1_RXOFF_STAY_RX,  //Main Radio Control State Machine Configuration
    MCSM0   , 0x18,     //Main Radio Control State Machine Configuration
    FOCCFG  , 0x16,     //Frequency Offset Compensation Configuration
    WORCTRL , 0xFB,     //Wake On Radio Control
    FSCAL3  , 0xE9,     //Frequency Synthesizer Calibration
    FSCAL2  , 0x2A,     //Frequency Synthesizer Calibration
    FSCAL1  , 0x00,     //Frequency Synthesizer Calibration
    FSCAL0  , 0x1F,     //Frequency Synthesizer Calibration
    TEST2   , 0x81,     //Various Test Settings
    TEST1   , 0x35,     //Various Test Settings
    TEST0   , 0x09,     //Various Test Settings
};
/*
//adr-------value
IOCFG2,		0x6,//Asserts when sync word has been sent / received, and de-asserts at the end of the packet. In RX, the pin will de-assert
//when the optional address check fails or the RX FIFO overflows. In TX the pin will de-assert if the TX FIFO underflows.
IOCFG0,		0x3F,
FIFOTHR,	0x07,
PKTLEN,		CC1101_FIXED_PACKET_LENGTH,
PKTCTRL1,	PKTCTRL1_ADR_CHK_NO_BROADCAST | PKTCTRL1_APPEND_STATUS | PKTCTRL1_CRC_AUTOFLUSH,
PKTCTRL0,	PKTCTRL0_CRC_EN,  //0x44,
ADDR,		CC1101_ADDRESS,
CHANNR,		0x00,
FSCTRL1,	0x0A,
FSCTRL0,	0x00,
FREQ2,		0x1E,
FREQ1,		0xC4,
FREQ0,		0xEC,
MDMCFG4,	0x8C,
MDMCFG3,	0x22,
MDMCFG2,	0x9,//0x12,
MDMCFG1,	0x82,//0x22,
MDMCFG0,	0xF8,
DEVIATN,	0x47,
MCSM0,		0x19,//0x08,
FOCCFG,		0x36,
BSCFG,		0x6C,
AGCCTRL2,	0x03,
AGCCTRL1,	0x40,
AGCCTRL0,	0x91,
FREND1,		0x56,
FREND0,		0x10,
FSCAL3,		0xA9,
FSCAL2,		0x0A,
FSCAL1,		0x00,
FSCAL0,		0x0D,
FSTEST,		0x57,
TEST2,		0x98,
TEST1,		0x31,
TEST0,		0x0B,
*/
void cc1101_init(void){
	uint32_t i =0;
	uint16_t massive_size = sizeof(cc1101_register_config);
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	gpio_x_pin_y_config(CC1101_GPIO, CC1101_GDO_2_PIN, GPIO_CR_MODE_INPUT, GPIO_CR_CNF_INPUT_FLOATING);

	SPI1_NSS_LOW();
	while(SPI1_GPIO->IDR & (1<<SPI1_PIN_MISO)){}//ждем пока сс1101 опустит МИСО
	cc1101_command_strobe(CC1101_STROBE_SRES,0);
	SPI1_NSS_HIGHT();
	for(i = 0; i< 7200000; i++){
		asm("nop");
	}
	SPI1_NSS_LOW();
	while(SPI1_GPIO->IDR & (1<<SPI1_PIN_MISO)){}//ждем пока сс1101 опустит МИСО
	for(i = 0; i < massive_size; i+=2){
		cc1101_data_send(cc1101_register_config[i], cc1101_register_config[i+1]);
	}
	cc1101_data_send(CC1101_PATABLE, CC1101_PA_POWER_0);
	cc1101_command_strobe(CC1101_STROBE_SIDLE,0);
	cc1101_command_strobe(CC1101_STROBE_SFRX,0);
	cc1101_command_strobe(CC1101_STROBE_SFTX,0);
}
uint8_t cc1101_paket_crc_check(uint8_t * packet_ptr){
	uint8_t tmp = 0;
	uint8_t crc_ok = 0;
	tmp = packet_ptr[(CC1101_FIXED_PACKET_LENGTH + CC1101_LQI_CRC_BYTE_OFFSET) -1];//место хранения CRC
	crc_ok =tmp & CC1101_CRC_OK;
	return crc_ok;
}
uint8_t cc1101_data_send(uint8_t reg_addres,uint8_t tx_data){
	uint8_t rx_data = 0;
	uint8_t header_byte = reg_addres;// = (reg_addres & 0x3f);//берем младшие 6 бит. тк только они значимы.

	SPI1_NSS_LOW();
	//SPI1->CR1 = SPI1->CR1 & (~SPI_CR1_SSI);
	while(SPI1_GPIO->IDR & (1<<SPI1_PIN_MISO)){}//ждем пока сс1101 опустит МИСО
	spi_basic_byte_send(header_byte);
	rx_data = spi_basic_byte_send(tx_data);
	//SPI1->CR1 |= SPI_CR1_SSI;
	SPI1_NSS_HIGHT();
	return rx_data;
}

int16_t cc1101_command_strobe(uint8_t comand, uint8_t read_nwrite_bit){
	uint8_t rx_data = 0;
	uint8_t strobe_byte = comand;// = (comand & 0x3f);//берем младшие 6 бит. тк только они значимы.
	if(strobe_byte < CC1101_STROBE_ADDRES_MIN || strobe_byte > CC1101_STROBE_ADDRES_MAX)
		return CC1101_STROBE_ADRESS_ERROR;
	if(read_nwrite_bit) strobe_byte |= CC1101_READ_nWRITE_BIT;

	SPI1_NSS_LOW();
	while(SPI1_GPIO->IDR & (1<<SPI1_PIN_MISO)){}//ждем пока сс1101 опустит МИСО
	rx_data = spi_basic_byte_send(strobe_byte);
	SPI1_NSS_HIGHT();
	return rx_data;
}

uint8_t cc1101_data_read(uint8_t reg_addres){//возвращает бат статуса в старших битах. и байт запрошенный в младших 8ми
	uint8_t rx_data = 0;
	uint8_t status_reg = 0;
	const uint8_t garbage = 0;
	uint8_t header_byte = reg_addres;// = (reg_addres & 0x3f);//берем младшие 6 бит. тк только они значимы.
	header_byte |= CC1101_READ_nWRITE_BIT;

	SPI1_NSS_LOW();
	while(SPI1_GPIO->IDR & (1<<SPI1_PIN_MISO)){}//ждем пока сс1101 опустит МИСО
	status_reg = spi_basic_byte_send(header_byte);
	rx_data = spi_basic_byte_send(garbage);
	SPI1_NSS_HIGHT();
	return rx_data;//(status_reg << 8) |
}

uint8_t cc1101_data_read_burst(uint8_t reg_addres, uint8_t * data_ptr,uint8_t burst_size){//возвращает бат статуса
	uint8_t i;
	uint8_t status_reg = 0;
	const uint8_t garbage = 0;
	uint8_t header_byte = reg_addres;//  = (reg_addres & 0x3f);//берем младшие 6 бит. тк только они значимы.
	header_byte |= CC1101_READ_nWRITE_BIT;
	header_byte |= CC1101_BURST_ACCESS_BIT;
	SPI1_NSS_LOW();
	while(SPI1_GPIO->IDR & (1<<SPI1_PIN_MISO)){}//ждем пока сс1101 опустит МИСО
	status_reg = spi_basic_byte_send(header_byte);
	for(i = 0; i < burst_size;i++)
		data_ptr[i] = spi_basic_byte_send(garbage);
	SPI1_NSS_HIGHT();
	return status_reg;
}

uint8_t cc1101_wait_until_iddle_state(){
	uint8_t cc1101_status_byte = 0;
	do{
				//пока трансивер не перейдет в ожидание\ ждем
				cc1101_status_byte = cc1101_command_strobe(CC1101_STROBE_SNOP,1);
	}
	while((cc1101_status_byte & CC1101_STATUS_STATE_MASK) != CC1101_STATUS_STATE_IDDLE);
	return cc1101_status_byte;
}

uint8_t cc1101_data_write_burst(uint8_t reg_addres, uint8_t * data_ptr,uint8_t burst_size){//возвращает байт статуса
	uint8_t i;
	uint8_t status_reg = 0;
	uint8_t header_byte = reg_addres;//  = (reg_addres & 0x3f);//берем младшие 6 бит. тк только они значимы.
	BIT_VALUE_RESET(header_byte,CC1101_READ_nWRITE_BIT);
	header_byte |= CC1101_BURST_ACCESS_BIT;
	SPI1_NSS_LOW();
	while(SPI1_GPIO->IDR & (1<<SPI1_PIN_MISO)){}//ждем пока сс1101 опустит МИСО
	status_reg = spi_basic_byte_send(header_byte);
	for(i = 0; i < burst_size;i++)
		status_reg = spi_basic_byte_send(data_ptr[i]);
	SPI1_NSS_HIGHT();
	return status_reg;
}

uint8_t cc1101_tx_send_single_byte(uint8_t byte){
	return cc1101_data_send(CC1101_TX_FIFO_SINGL,byte);
}
uint8_t cc1101_rx_read_single_byte(){
	return cc1101_data_read(CC1101_RX_FIFO_SINGL);
}

int16_t cc1101_rssi_convert_to_dbm(uint8_t rssi){
	int16_t rssi_dbm = 0;
	int16_t rssi_dec = rssi;

	if(rssi >= 128)//по датащиту
		rssi_dec -= 256;
	rssi_dbm = (rssi_dec)/2 - CC1101_RSSI_OFFSET;
	return rssi_dbm;
}
#endif
