#ifndef SI4432_C
#define SI4432_C

#include "si4432.h"

#define IT_ICHIPRDY (1<<1)
#define SI4432_IT_IPKSENT (1<<2)
#define SI4432_IT_IPKVALID (1<<1)
uint8_t si4432_interrupt = 0;

// -- скорость обмена ---------------------------------------------------------

uint8_t Config[1][2] = { { SI4432_REG_Clock_Recovery_Gearshift_Override, 0x03 } };

uint8_t si4432_reg[] = {
0x1C,	0x05, //S2 9C05
0x1D,	0x00, //S2 9D00
0x20,	0xD0,	//S2 A0D0
0x21,	0xE0,	//S2 A1E0
0x22,	0x08,	//S2 A208
0x23,	0x31,	//S2 A331
0x24,	0x00,	//S2 A400
0x25,	0x03,	//S2 A503
0x2A,	0x20,	//S2 AA20

0x30,	0xAC,	//S2 B0AC
0x33,	0x2A,	//S2 B32A
0x34,	0x04,	//S2 B404
0x35,	0x20,	//S2 B520
0x3A,	0xD2,	//S2 BAD2
0x3B,	0x4B,	//S2 BB4B
0x3E,	SI4432_PACKET_LENGHT,	//S2 BE04
0x3F,	0xD2,	//S2 BFD2
0x40,	0x4B,	//S2 C04B
0x43,	0xD2,	//S2 C3D2
0x44,	0x4B,	//S2 C44B

0x6E,	0x08,	//S2 EE08
0x6F,	0x31,	//S2 EF31

0x70,	0x23,	//S2 F023
0x71,	0x23,	//S2 F123
0x72,	0x50,	//S2 F250

0x75,	0x53,	//S2 F553
0x76,	0x4B,	//S2 F64B
0x77,	0x00,	//S2 F700
0, 0,
};


//-----------------------------------------------------------------------------
/// Передача пакета
//-----------------------------------------------------------------------------

int16_t si4432_tx_packet(uint8_t *data_tx_ptr){
	uint8_t interrupt1 =0;
	uint8_t interrupt2 =0;
	uint8_t i = 0;
	if(!(data_tx_ptr ))//если аргументы 0
		return ERROR_WRONG_ADDRESS;
	si4432_write_register(SI4432_REG_Interrupt_Enable1, SI4432_IT_IPKSENT);
	si4432_write_register(SI4432_REG_Interrupt_Enable2, 0);
	// read interrupt status
	interrupt1 = si4432_read_register(SI4432_REG_Interrupt_Status1);
	interrupt2 =si4432_read_register(SI4432_REG_Interrupt_Status2);
	//delay_ms(1000);
	for(i = 0; i < SI4432_PACKET_LENGHT; i++){//заполняем фифо на отправку
		si4432_write_register(SI4432_REG_FIFO_Access,data_tx_ptr[i]);
	}
	//включаем передачу пакета

	si4432_write_register(SI4432_REG_Operating_Function_Control1,SI4432_REG0x7_TX_ON);
	while( SI4432_IS_nIRQ()){};//ждем отправки
		BLUE_PILL_LED_OFF();//выключение светодиода
		delay_ms(200);
		BLUE_PILL_LED_ON();//включение светодиода
		delay_ms(200);

	interrupt1 = si4432_read_register(SI4432_REG_Interrupt_Status1);
	interrupt2 =si4432_read_register(SI4432_REG_Interrupt_Status2);

}

//-----------------------------------------------------------------------------
/// Прием пакета за указанное время
//-----------------------------------------------------------------------------

int16_t si4432_rx_packet(uint8_t *data_tx_ptr, uint16_t time_to_rx){
	uint8_t interrupt1 =0;
	uint8_t interrupt2 =0;
	//uint8_t pk_len = 0;
	uint8_t i = 0;
	if(time_to_rx == 0) time_to_rx = 2000;//2sec to rx
	if(!(data_tx_ptr ))//если аргументы 0
		return ERROR_WRONG_ADDRESS;
	si4432_write_register(SI4432_REG_Interrupt_Enable1, SI4432_IT_IPKVALID);
	si4432_write_register(SI4432_REG_Interrupt_Enable2, 0);
	// read interrupt status
	interrupt1 = si4432_read_register(SI4432_REG_Interrupt_Status1);
	interrupt2 =si4432_read_register(SI4432_REG_Interrupt_Status2);
	//включаем прием пакета
	si4432_write_register(SI4432_REG_Operating_Function_Control1,SI4432_REG0x7_RX_ON);
	tim2_time_ms(time_to_rx);
	while(SI4432_IS_nIRQ() && (!global_main_flag_tim2_update)){//
		BLUE_PILL_LED_OFF();//выключение светодиода
		delay_ms(200);
		BLUE_PILL_LED_ON();//включение светодиода
		delay_ms(800);
	};//ждем приема или пока не истечет таймер
	//go to iddle
	si4432_write_register(SI4432_REG_Operating_Function_Control1, REGx07_xton);
	interrupt1 = si4432_read_register(SI4432_REG_Interrupt_Status1);
	interrupt2 =si4432_read_register(SI4432_REG_Interrupt_Status2);
	//pk_len = si4432_read_register(SI4432_REG_Received_Packet_Length);
	if(interrupt1 & SI4432_IT_IPKVALID){
		for(i = 0; i < SI4432_PACKET_LENGHT; i++){//читаем принятую информацию
			data_tx_ptr[i] = si4432_read_register(SI4432_REG_FIFO_Access);
		}
		return SI4432_PACKET_LENGHT;
	}
	else
		return 0;
}
//-----------------------------------------------------------------------------
/// Начальная инициализация приемо-передатчика
//-----------------------------------------------------------------------------
int16_t si4432_init(void)
{
	uint8_t chip_type =0;
	uint8_t chip_ver = 0;

	uint8_t interrupt1 =0;
	uint8_t interrupt2 =0;
	//конфигурируем светодиод
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
	gpio_x_pin_y_config(BLUE_PILL_LED_GPIO, BLUE_PILL_LED_PIN, GPIO_CR_MODE_OUTPUT_SPEED_10MHZ, GPIO_CR_CNF_OUTPUT_OPEN_DRAIN);
	BLUE_PILL_LED_OFF();//выключение светодиода
	//необходимые ножки для работы
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;//тактирование GPIOA
	gpio_x_pin_y_config(SI4432_GPIO, Pin_SI4432_SDN, GPIO_CR_MODE_OUTPUT_SPEED_50MHZ, GPIO_CR_CNF_OUTPUT_OPEN_DRAIN);
	gpio_x_pin_y_config(SI4432_GPIO, Pin_SI4432_nIRQ, GPIO_CR_MODE_INPUT, GPIO_CR_CNF_INPUT_FLOATING);

	delay_ms(100);
	GPIO_PIN_SET(SI4432_GPIO, Pin_SI4432_SDN);	// сбрасываем чип
	delay_ms(10000);
	GPIO_PIN_RESET(SI4432_GPIO, Pin_SI4432_SDN);//включаем чип
	delay_ms(10000);
	// read interrupt status
	interrupt1 = si4432_read_register(SI4432_REG_Interrupt_Status1);
	interrupt2 =si4432_read_register(SI4432_REG_Interrupt_Status2);
	// SW reset -> wait for POR interrupt
	si4432_write_register(SI4432_REG_Operating_Function_Control1, 0x80);//программный сброс
	delay_ms(1000);
	si4432_write_register(SI4432_REG_Operating_Function_Control1, REGx07_xton);
	delay_ms(1000);
	// Enable the POR interrupt

	while( SI4432_IS_nIRQ());// Wait for the POR interrupt
	chip_type = si4432_read_register(SI4432_REG_Device_Type);
	chip_ver  = si4432_read_register(SI4432_REG_Device_Version);
	// read interrupt status
	interrupt1 =si4432_read_register(SI4432_REG_Interrupt_Status1);
	interrupt2 =si4432_read_register(SI4432_REG_Interrupt_Status2);

	if(interrupt2  == 0x03){//power on reset and ichiprdy(xtall on)
		si4432_write_register(SI4432_REG_Operating_Function_Control1, 0x00);
		BLUE_PILL_LED_ON();//включение светодиода
	}
	else{
		GPIO_PIN_SET(SI4432_GPIO, Pin_SI4432_SDN);
		for(uint8_t i =0; i<10;i++){
			//мигаем светодиодом 19раз как знак ошибки
			BLUE_PILL_LED_ON();//включение светодиода
			delay_ms(500);
			BLUE_PILL_LED_OFF();//включение светодиода
			delay_ms(500);
		}
		return ERROR_NO_RESPONSE;
	}
	si4432_write_register(SI4432_REG_LDO_Control_Override, 0xA1);
	// disable all ITs, except 'ichiprdy'
	si4432_write_register(SI4432_REG_Interrupt_Enable1, 0x00);
	si4432_write_register(SI4432_REG_Interrupt_Enable2, IT_ICHIPRDY);
	si4432_read_register(SI4432_REG_Interrupt_Status1);
	si4432_read_register(SI4432_REG_Interrupt_Status2);
	for(int i =0; si4432_reg[i] !=0;i+=2){
		si4432_write_register(si4432_reg[i],si4432_reg[i +1]);
	}
	// set the non-default Si4432 registers
	// set TX Power +11dBm
	si4432_write_register(SI4432_REG_TX_Power, 0x00);
	//автопереключение антены прием\передача
	si4432_write_register(SI4432_REG_GPIO0_Configuration, 0x12);// set GPIO0 to TX State
	si4432_write_register(SI4432_REG_GPIO1_Configuration, 0x15);// set GPIO1 to RX State
	return ERROR_NO_ERROR;
}

//-----------------------------------------------------------------------------
///
//-----------------------------------------------------------------------------
void si4432_idle(void){
	si4432_write_register(SI4432_REG_Operating_Function_Control1, 0x00);// disable transmitter and receiver
	// disable all ITs
	si4432_write_register(SI4432_REG_Interrupt_Enable1, 0x00);
	si4432_write_register(SI4432_REG_Interrupt_Enable2, 0x00);
	// read the interrupt status registers from the radio to clear the IT flags
	si4432_read_register(SI4432_REG_Interrupt_Status1);
	si4432_read_register(SI4432_REG_Interrupt_Status2);
}
//-----------------------------------------------------------------------------
///
//-----------------------------------------------------------------------------
void si4432_transmit(uint8_t * packet,uint8_t lenght){
	uint8_t i = 0;
	for ( i = 0; i < lenght; i++){
		si4432_write_register(SI4432_REG_FIFO_Access, packet[i]);
	}
	//si4432_write_register(SI4432_REG_LDO_Control_Override, 0xA1);// errata
	delay_ms(5);//
	si4432_write_register(SI4432_REG_Operating_Function_Control1, 0x09);// enable transmitter
	delay_ms(5);//
	si4432_write_register(SI4432_REG_Interrupt_Enable1, 0x04);// enable the packet sent interrupt only
	// read interrupt status registers
	si4432_read_register(SI4432_REG_Interrupt_Status1);
	si4432_read_register(SI4432_REG_Interrupt_Status2);
	while (!(SI4432_GPIO->IDR & (1<<Pin_SI4432_nIRQ))){}// wait for the packet sent interrupt
}

//-----------------------------------------------------------------------------
///
//-----------------------------------------------------------------------------
void si4432_receive(void){
	si4432_write_register(SI4432_REG_LDO_Control_Override, 0xA1);// errata
	delay_ms(5);//
	si4432_write_register(SI4432_REG_Operating_Function_Control1, 0x05);// enable receiver chain
	delay_ms(5);//
	// enable the wanted ITs
	si4432_write_register(SI4432_REG_Interrupt_Enable1, 0x13);
	si4432_write_register(SI4432_REG_Interrupt_Enable2, 0x00);
	// read interrupt status registers
	si4432_read_register(SI4432_REG_Interrupt_Status1);
	si4432_read_register(SI4432_REG_Interrupt_Status2);
}

//-----------------------------------------------------------------------------
///
//-----------------------------------------------------------------------------
SI4432_ENUM si4432_packet_received(uint8_t * rx_data){
	uint8_t interrupt_status = 0;
	uint8_t i =0;
	uint8_t data_lenght = 0;
	if (SI4432_GPIO->IDR & (1<<Pin_SI4432_nIRQ)){// Check if IT occurred or not
		/* check what caused the interrupt */
		interrupt_status = si4432_read_register(SI4432_REG_Interrupt_Status1);// read out IT status register
		si4432_read_register(SI4432_REG_Interrupt_Status2);

		if ((interrupt_status & 0x02) == 0x02){// packet received interrupt occurred
			data_lenght = si4432_read_register(SI4432_REG_Received_Packet_Length);// read buffer
			for (i = 0; i < data_lenght; i++){
				rx_data[i] = si4432_read_register(SI4432_REG_FIFO_Access);
			}
			si4432_write_register(SI4432_REG_Operating_Function_Control1, 0x01);// disable receiver
			return SI4432_RF_PACKET_RECEIVED;
		}

		if ((interrupt_status & 0x01) == 0x01){// CRC ERROR interrupt occurred
			si4432_write_register(SI4432_REG_Operating_Function_Control1, 0x01);// disable receiver
			return SI4432_RF_CRC_ERROR;
		}
	}
	return SI4432_RF_NO_PACKET;
}
//-----------------------------------------------------------------------------
/// Чтение регистра
//-----------------------------------------------------------------------------
uint8_t si4432_read_register(uint8_t address){
	uint8_t garbage = 0;
	uint8_t tmp =0;
	GPIO_PIN_RESET(SI4432_GPIO, Pin_SI4432_nSEL);	// выбор чипа
	spi_basic_byte_send(address);// запись адреса
	tmp = spi_basic_byte_send(garbage);// чтение регистра
	GPIO_PIN_SET(SI4432_GPIO, Pin_SI4432_nSEL);// деактивация чипа
	return tmp;
}
//-----------------------------------------------------------------------------
/// Запись регистра
//-----------------------------------------------------------------------------
void si4432_write_register(uint8_t address, uint8_t value){
	GPIO_PIN_RESET(SI4432_GPIO, Pin_SI4432_nSEL);// выбор чипа
	spi_basic_byte_send(address | SI4432_BIT_WRITE_nREAD);// запись адреса
	spi_basic_byte_send(value);//запись значения
	GPIO_PIN_SET(SI4432_GPIO, Pin_SI4432_nSEL);// деактивация чипа
}


#endif

