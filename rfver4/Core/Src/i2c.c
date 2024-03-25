#ifndef I2C_C
#define I2C_C
#include "i2c.h"

void i2c_init(void) {
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN; //Запуск тактирование порта B
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN; //Запуск тактирования альтернативных функций
	RCC->APB1ENR |= RCC_APB1ENR_I2C1EN; //Запуск тактирования I2C1

	gpio_x_pin_y_config(I2C_SCL_GPIO, I2C_SCL_PIN, GPIO_CR_MODE_OUTPUT_SPEED_50MHZ, GPIO_CR_CNF_ALTERNATE_FUNCTION_OPEN_DRAIN);
	gpio_x_pin_y_config(I2C_SDA_GPIO, I2C_SDA_PIN, GPIO_CR_MODE_OUTPUT_SPEED_50MHZ, GPIO_CR_CNF_ALTERNATE_FUNCTION_OPEN_DRAIN);

	//i2c_reset();
	I2Cx_ACTIVE->CR2 = CPU_CLOCK_MHZ << I2C_CR2_FREQ_Pos; //freq for i2c
#ifdef IS_SLAVE
	I2Cx_ACTIVE->OAR1 = 35	<< I2C_OAR1_ADD0_Pos;//max5980 address, try emulate
#endif
#ifndef IS_SLAVE
	I2Cx_ACTIVE->OAR1 = DEVICE_ADDR_7_BIT	<< I2C_OAR1_ADD0_Pos;//device address1
#endif
	I2Cx_ACTIVE->OAR2 = 0;//device address2
//I2C Clock control register
	//select mode slom mode or fast mode SM | FM
	//and select timing in Fast mode low lewel to high level __-  equal 2 or 16:9
#ifdef I2C_SM//Standard mode I2C
	BIT_VALUE_RESET(I2Cx_ACTIVE->CCR, I2C_CCR_FS);
#endif
#ifndef I2C_SM//Fast mode I2C
	I2Cx_ACTIVE->CCR |= I2C_CCR_FS;
	BIT_VALUE_RESET(I2Cx_ACTIVE->CCR, I2C_CCR_DUTY); //tlow/thigh = 2
	#ifdef I2C_FM_DUTY_16_9
	I2Cx_ACTIVE->CCR |= I2C_CCR_DUTY;//tlow/thigh = 16:9
	#endif
#endif
	I2Cx_ACTIVE->CCR |= I2C_CCR << I2C_CCR_CCR_Pos;//f_i2c to real speed ratio

	I2Cx_ACTIVE->TRISE |= I2C_TRISE_TIME << I2C_TRISE_TRISE_Pos;//t rise front signal
	I2Cx_ACTIVE->CR1 |= I2C_CR1_PE;//i2c1 enable
}


//send address for 7bit case
int16_t i2c_send_address(uint8_t address, uint8_t READ_nwrite){
	address = I2C_ADDRESS_W_Nr(address,I2C_READ);
	BIT_VALUE_RESET(I2Cx_ACTIVE->CR1, I2C_CR1_POS);//(N)ACK for CURRENT byte in the shift register
	I2C_START();//send START

	while (!(I2Cx_ACTIVE->SR1 & I2C_SR1_SB)) {
		delay_ms(50);//if start not happend
		return -1;
	}//wait for START happend
	//clear SB & send address
	I2Cx_ACTIVE->SR1;
	I2Cx_ACTIVE->DR = (address);
	//
	//if  ACK correct and address send
	while ( (I2Cx_ACTIVE->SR1 & I2C_SR1_AF) && !(I2Cx_ACTIVE->SR1 & I2C_SR1_ADDR)) {
		delay_ms(50);//if NACK
		return -1;
	}
	I2C_CLEAR_ADDR();
	return 0;
}

int16_t i2c_read_data(uint8_t address, uint8_t *data, uint8_t lenght){
	uint8_t i = 0;
	int16_t result = 0;
//start
	i2c_send_address(address, I2C_READ);
//read data stream
	for(; i < lenght - 1; i++) {
		I2C_SET_ACK(); //send ACK
		result = i2c_rx();//wait for BYTE
		if(result < 0) return result;
		data[i] = result;
	}
//The last one byte with NACK and STOP signal
	I2C_SET_n_ACK(); //NACK
	I2C_STOP();//send STOP
	result = i2c_rx();//wait for BYTE
	if(result < 0) return result;
	data[i] = result;
	return 0;
}

int16_t i2c_send_data(uint8_t address,uint8_t *data, uint8_t lenght){
	uint8_t i = 0;
	int16_t correct = 0;
	BIT_VALUE_RESET(I2Cx_ACTIVE->CR1, I2C_CR1_POS); //(N)ACK for CURRENT byte in the shift register
	i2c_send_address(address, I2C_WRITE);
	if(correct < 0) return correct;
	//send data
	for (; i < lenght; i++){
			while (!(I2Cx_ACTIVE->SR1, I2C_SR1_TXE)) {//while data reg NOT EMPTY

				if (I2Cx_ACTIVE->SR1 & I2C_SR1_AF) {//if ACK Failure
					I2C_STOP();//send STOP
					BIT_VALUE_RESET(I2Cx_ACTIVE->SR1, I2C_SR1_AF);//AF = ACK Failure
					return -1;
				}
			}
			I2Cx_ACTIVE->DR = data[i]; //Запись байта
		}
		I2C_STOP();//send STOP
		return 0;
}


//
int16_t i2c_send_and_read(uint8_t address,uint8_t reg_num){
	int16_t correct = 0;
	BIT_VALUE_RESET(I2Cx_ACTIVE->CR1, I2C_CR1_POS); //(N)ACK for CURRENT byte in the shift register
	correct = i2c_send_address(address, I2C_WRITE);
	if(correct < 0) return correct;
	//send register number
	while (!(I2Cx_ACTIVE->SR1, I2C_SR1_TXE)) {//while data reg NOT EMPTY
		if (I2Cx_ACTIVE->SR1 & I2C_SR1_AF) {//if ACK Failure
			I2C_STOP();//send STOP
			BIT_VALUE_RESET(I2Cx_ACTIVE->SR1, I2C_SR1_AF);//AF = ACK Failure
			return -1;
		}
	}
	I2Cx_ACTIVE->DR = reg_num; //Запись байта
	//The last one byte with NACK and STOP signal
	I2C_SET_n_ACK(); //NACK
	I2C_STOP();//send STOP
	return i2c_rx();//wait for BYTE
}


int16_t i2c_tx(uint8_t data){
	while(!(I2Cx_ACTIVE->SR1 & I2C_SR1_TXE)){
		delay_ms(50);//if nothing is happening
		return -1;
	}//wait for  empty data reg
	I2Cx_ACTIVE->DR = data;
	return 0;
}

int16_t i2c_rx(){
	while(!(I2Cx_ACTIVE->SR1 & I2C_SR1_RXNE)){
		delay_ms(50);//if no data on line
		return -1;
	}//wait for not empty data reg
	return I2Cx_ACTIVE->DR;
}
/*
void i2c_slave_read_data(uint8_t address, uint8_t *data, uint8_t lenght){
	uint8_t i = 0;
//start
	BIT_VALUE_RESET(I2Cx_ACTIVE->CR1, I2C_CR1_POS);//(N)ACK for CURRENT byte in the shift register
	while (!(I2Cx_ACTIVE->SR1 & I2C_SR1_SB)) {
		if(I2Cx_ACTIVE->SR1 & I2C_SR1_BERR){
			I2Cx_ACTIVE->SR1;
			I2Cx_ACTIVE->CR1 = I2C_CR1_ACK |  I2C_CR1_PE;
			I2Cx_ACTIVE->SR1 = 0;
		}
	}//wait for START happend (mb endless loop)
	//clear SB & send address
	//I2Cx_ACTIVE->SR1;
	//
	//if address received
	while ( !(I2Cx_ACTIVE->SR1 & I2C_SR1_ADDR)) {
		if(I2Cx_ACTIVE->SR1 & I2C_SR1_BERR){
			I2Cx_ACTIVE->SR1;
			I2Cx_ACTIVE->CR1 = I2C_CR1_ACK |  I2C_CR1_PE;
			I2Cx_ACTIVE->SR1 = 0;
		}
		if(I2Cx_ACTIVE->SR1 & I2C_SR1_STOPF){
			I2Cx_ACTIVE->SR1 = 0;
		}
	}
	I2C_CLEAR_ADDR();
//read data stream
	for(; i < lenght - 1; i++) {
		//I2C_SET_ACK(); //send ACK
		data[i] = i2c_rx();//wait for BYTE (mb endless loop)
	}
//The last one byte with NACK and STOP signal
	//I2C_SET_n_ACK(); //NACK
	//I2C_STOP();//send STOP
	data[i] = i2c_rx();//wait for BYTE (mb endless loop)

}
*/
#endif


