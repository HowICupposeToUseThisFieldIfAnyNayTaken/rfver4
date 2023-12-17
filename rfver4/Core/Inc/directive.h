#ifndef DIRECTIVE_H
#define DIRECTIVE_H

#define BYTE_SIZE 8
#define DEC 10
#define HEX 16
#define STM32F103x6

#include "stm32f1xx.h"

#define CPU_CLOCK_MHZ 36/*625*25*9*256*/
#define FREQUENCY_KHZ 1000
#define FREQUENCY_MHZ (1000 * FREQUENCY_KHZ)
#define CPU_CLOCK CPU_CLOCK_MHZ * FREQUENCY_MHZ/* 8 MHz*/

#define ADC_2_FREQUENCY_RATIO 10
#define SAMPLING_FREQUENCY	(CPU_CLOCK / ADC_2_FREQUENCY_RATIO )
#define SAMPLES_PER_10US 	((10*SAMPLING_FREQUENCY) / FREQUENCY_MHZ)
#define SAMPLES_PER_100US 	((100*SAMPLING_FREQUENCY) / FREQUENCY_MHZ)
#define SAMPLES_PER_1MS 	(SAMPLING_FREQUENCY / FREQUENCY_KHZ)
#define SAMPLES_PER_10MS 	((10*SAMPLING_FREQUENCY) / FREQUENCY_KHZ)
#define SAMPLES_PER_100MS 	((100*SAMPLING_FREQUENCY) / FREQUENCY_KHZ)
#define SAMPLES_PER_S  		SAMPLING_FREQUENCY0


#define US_DELAY CPU_CLOCK / 1000000
#define GPIO_ANALOG_VCC 3000/*максимальное значение в милливольтах */
//gpio
#define GPIO_CR_CNF_INPUT_ANALOG					0x00/*00*/
#define GPIO_CR_CNF_INPUT_FLOATING 					0x01/*01*/
#define GPIO_CR_CNF_INPUT_PULL_UP_DOWN				0x02/*10*/
#define GPIO_CR_CNF_OUTPUT_PUSH_PULL 				0x00/*00*/
#define GPIO_CR_CNF_OUTPUT_OPEN_DRAIN 				0x01/*01*/
#define GPIO_CR_CNF_ALTERNATE_FUNCTION_PUSH_PULL	0x02/*10*/
#define GPIO_CR_CNF_ALTERNATE_FUNCTION_OPEN_DRAIN 	0x03/*11*/

#define GPIO_CR_MODE_INPUT						0x00/*00*/
#define GPIO_CR_MODE_OUTPUT_SPEED_10MHZ			0x01/*00*/
#define GPIO_CR_MODE_OUTPUT_SPEED_2MHZ			0x02/*01*/
#define GPIO_CR_MODE_OUTPUT_SPEED_50MHZ			0x03/*11*/


#define GPIO_BSRR_BIT_SET 	0	/*смещение для установки бита*/
#define GPIO_BSRR_BIT_RESET 16/*смещение для сброса бита*/

#define GPIO_PIN_SET(GPIOx,PIN_y) {GPIOx->BSRR = 1<<(PIN_y + GPIO_BSRR_BIT_SET);}
#define GPIO_PIN_RESET(GPIOx,PIN_y) {GPIOx->BSRR = 1<<(PIN_y + GPIO_BSRR_BIT_RESET);}

#define BIT_NUMBER_RESET(var, bit_number) {var = var & (~(1<<bit_number));}
#define BIT_NUMBER_SET(var, bit_number) {var |= 1<<bit_number;}
#define BIT_VALUE_RESET(var, value) {var = var & (~(value));}
#define BIT_VALUE_SET(var, value) {var |= value;}

#define ERROR_NO_ERROR 0
#define ERROR_WRONG_ADDRESS -1
#define BUFFER_OVERFLOW -2
#define ERROR_NO_RESPONSE -3
#define ERROR_WRONG_PARAMETRS -4

#define ADC_DR_POINTER 0x40012400 + 0x40
#define ADC_START() ADC1->CR = ADC_CR_ADSTART /*стартуем оцифровку*/
#define ADC_STOP() ADC1->CR |=ADC_CR_ADSTP /*приостанавливаем оцифровку*/

#define BLUETOOTH_PIN_nENABLE PIN_11
#define BLUETOOTH_PIN_AT_COMMANT PIN_12
#define BLUETOOTH_ENABLE() {UART1_PORT->BSRR = 1<<(BLUETOOTH_PIN_nENABLE + GPIO_BSRR_BIT_RESET);}
#define BLUETOOTH_DIASABLE() {UART_PORT->BSRR = 1<<(BLUETOOTH_PIN_nENABLE + GPIO_BSRR_BIT_SET);}

#define PIN_0 0
#define PIN_1 1
#define PIN_2 2
#define PIN_3 3
#define PIN_4 4
#define PIN_5 5
#define PIN_6 6
#define PIN_7 7
#define PIN_8 8
#define PIN_9 9
#define PIN_10 10
#define PIN_11 11
#define PIN_12 12
#define PIN_13 13
#define PIN_14 14
#define PIN_15 15
//клавиатура
#define GPIO_KB_OUT GPIOC
#define GPIO_KB_IN  GPIOC
#define PIN_1_KB_OUT 4
#define PIN_2_KB_OUT 10
#define PIN_3_KB_OUT 11
#define PIN_4_KB_OUT 12
#define PIN_1_KB_IN  13
#define PIN_2_KB_IN  14
#define PIN_3_KB_IN  15
#define PIN_4_KB_IN  6

#define KEY_RESET
#define KEY_HORIZ_PLUS			8
#define KEY_HORIZ_MINUS 		7
#define KEY_VERTICAL_PLUS
#define KEY_VERTICAL_MINUS
#define KEY_HORIZ_OFFSET_PLUS  	3
#define KEY_HORIZ_OFFSET_MINUS 	4
#define KEY_VERT_OFFSET_PLUS 	9
#define KEY_VERT_OFFSET_MINUS	0xd
#define KEY_TRIGGER_TYPE
#define KEY_TRIGGER_LEVEL_PLUS	 1
#define KEY_TRIGGER_LEVEL_MINUS	 5


#define FONT_HEIGHT  16
#define BLUE_PILL_LED_GPIO GPIOC
#define BLUE_PILL_LED_PIN PIN_13
#define BLUE_PILL_LED_ON() {GPIO_PIN_RESET(BLUE_PILL_LED_GPIO,BLUE_PILL_LED_PIN);}
#define BLUE_PILL_LED_OFF() {GPIO_PIN_SET(BLUE_PILL_LED_GPIO,BLUE_PILL_LED_PIN);}



#define  swap(a,b){int32_t tmp = a; a = b; b = tmp;}
#define wait_1us() asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop")
#define clk_delay(clks) for(unsigned long int i = 0;i < clks;i++){}
#define us_delay(t_us) clk_delay(t_us*(US_DELAY))
//
struct date_format{
	uint16_t year;
	uint8_t	month;
	uint8_t	day;
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
	uint16_t sub_seconds;
};

//

extern uint32_t global_relative_time;
extern uint32_t global_time_is_real;

void date_increment(struct date_format * s_date);
void gpio_x_pin_y_config(GPIO_TypeDef * GPIOx,uint8_t pin_y, uint8_t moder_and_speed, uint8_t cnf );
char hex2char(uint8_t num);
void hex32_2str(uint32_t hex, char * str);
void num32_2str(uint32_t num,uint8_t base, char *str);
void delay_ms(uint16_t msec);
///

//extern char str_recieve[UART_RX_BUFF_SIZE];
//
//#define delay_us(time) while(time){wait_1us();time--;}
#endif /* DIRECTIVE_H_ */
