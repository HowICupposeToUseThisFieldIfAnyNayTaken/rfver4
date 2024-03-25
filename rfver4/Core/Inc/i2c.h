#ifndef I2C_H
#define I2C_H
#include "directive.h"

//важно изменить директивы и задать временные константы
#define DEVICE_ADDR_7_BIT 27 /*own device address*/
//#define I2C_SM
#define I2C_FM
#define I2C_FM_DUTY_2
//#define I2C_FM_DUTY_16_9
//#define I2C_USE_I2C1
#define I2C_USE_I2C2

//timing block
#ifdef I2C_SM
	#define I2C_CCR 180
	#define I2C_TRISE_TIME 37
#endif

#ifdef I2C_FM
	#ifdef I2C_FM_DUTY_16_9
		#define I2C_CCR 4
	#endif
	#ifdef I2C_FM_DUTY_2
		#define I2C_CCR 30
	#endif
	#define I2C_TRISE_TIME   12
#endif
//end timing block



//device select
#ifdef I2C_USE_I2C1

#define I2Cx_ACTIVE I2C1

#define I2C_SCL_GPIO GPIOB
#define I2C_SCL_PIN PIN_6
#define I2C_SDA_GPIO GPIOB
#define I2C_SDA_PIN PIN_7

#endif


#ifdef I2C_USE_I2C2
#define I2Cx_ACTIVE I2C2

#define I2C_SCL_GPIO GPIOB
#define I2C_SCL_PIN PIN_10

#define I2C_SDA_GPIO GPIOB
#define I2C_SDA_PIN PIN_11

#endif
//end device select




#define I2C_START() {I2Cx_ACTIVE->CR1 |= I2C_CR1_START;}
#define I2C_STOP()	{I2Cx_ACTIVE->CR1 |= I2C_CR1_STOP;}

#define I2C_READ 1
#define I2C_WRITE 0

#define I2C_ADDRESS_W_Nr(addr, W_nR) (((addr)<<1) | (W_nR))
#define I2C_CLEAR_ADDR() {I2Cx_ACTIVE->SR1;I2Cx_ACTIVE->SR2;}
#define I2C_SET_ACK() {I2Cx_ACTIVE->CR1 |= I2C_CR1_ACK;}
#define I2C_SET_n_ACK() {BIT_VALUE_RESET(I2Cx_ACTIVE->CR1, I2C_CR1_ACK);}
void i2c_init();

int16_t i2c_rx();
int16_t i2c_tx(uint8_t data);

int16_t i2c_send_address(uint8_t address, uint8_t READ_nwrite);
int16_t i2c_send_data(uint8_t address, uint8_t *data, uint8_t lenght);
int16_t i2c_read_data(uint8_t address, uint8_t *data, uint8_t lenght);
int16_t i2c_send_and_read(uint8_t address,uint8_t reg_num);
int16_t i2c_slave_read_data(uint8_t address, uint8_t *data, uint8_t lenght);

#endif
