
#include "directive.h"
#include "i2c.h"
#include "tim.h"
#include "max5980.h"
#include "uart.h"
#include "dma.h"
#include "string.h"
/*
 *..    ／＞　 フ
　　　　　| 　_　 _|
　 　　　／`ミ _x 彡
　　 　 /　　　 　 |
　　　 /　 ヽ　　 ﾉ
　／￣|　　 |　|　|
　| (￣ヽ＿_ヽ_)_)
　＼二つ
i2c-Master: Контроллер STM32F103C8T6. - тестировалось на Ф103с6 тк с8 оказался не верифицируемым на подлинность -_- привет taggsm,
для корректной работы нужно найти хедер для с8, тк в с6 нет и2с2
(Тактирование внешнее от кварца на 8Мгц + часовой кварц. - внешний на 8 используется, разгоняется до 36Мгц, часовой кварц инициализирован
 и выходит на частоту(больше никак не используется)
Интерфейс отладки — SWD - этот интерфейс используется по умолчанию, скорее чтобы его убрать надо приложить немало усилий
I2C2 — PB10/PB11.
USART2 — PA2/PA3.)
юарт2 и спи2 легко переинициализируются на юарт1 и спи1, спи требует настройки времени и режима, константы для 36мгц забиты
юарт работает через дма, спи в циклах, для избежания бесконечных циклов в них встроен таймер который через 50мс выходит из функции с ошибкой
i2c-Slave: контроллер PoE MAX5980GTJ+. - в наличии не было, проверить в живую не удалось
Ознакомиться с возможностями по управлению контроллером PoE. - ознакомился, очень интересно.
Написать программу опроса состояния выходных портов контроллера и отправки сообщений - написано, опрашивает(в теории корректно).
об их состоянии по uart в читаемом виде (например: «DEBUG: PORT(1) — LOAD DETECT»).
 отправляет данные в формате DEBUG: PORT(1) device_detection device_class
 в сом терминале успешно игнорирует любые символы переноса строки

Функции обмена по i2c сделать отдельными файлами *.с/*.h сделано. много. отдельных. файлов.
 */
void interrupts_first_init();
void rcc_init();
/////////////////
int main(void) {
	uint8_t buff[64];
	uint8_t uart_tx[UART_TX_BUFF_SIZE]="test meow meow meow\n\0";
	uint8_t uart_rx[UART_RX_BUFF_SIZE];

	uint8_t buff_rx[2] = {0,0};

	buff[0] = 127;
	buff[1] = 6;
	const char* det_str = 0;
	const char* class_str = 0;
	rcc_init();
	tim2_init(TIM1_MSEC_PSK/2,2);//timer2 1ms for exit from endless loop
	i2c_init();
	interrupts_first_init();
	dma_init(2, UART_TX_BUFF_SIZE, uart_tx, UART_RX_BUFF_SIZE, uart_rx);
	uart2_init(uart_rx, uart_tx);
	//TIM2_ENABLE();

	while (1) {

		delay_ms(1000);

#ifdef IS_READY
		for(uint8_t port_n = 0; port_n < MAX_PORT_NUMBER; port_n++){
			char port_str[] = "DEBUG: PORT(N)\n\n\0";
			port_str[12]=hex2char(port_n);
			max_port_status_check(port_n, &det_str, &class_str);
			uart_transmit_string(2, port_str, str_len(port_str));
			delay_ms(100);
			uart_transmit_string(2, det_str, str_len(det_str));
			delay_ms(100);
			uart_transmit_string(2, class_str, str_len(class_str));
			delay_ms(100);
		}
#endif
#ifdef IS_MASTER
		i2c_send_data(MAX_DEVICE_ADDRESS,buff ,1);
#endif
#ifdef IS_SLAVE
		i2c_slave_read_data(MAX_DEVICE_ADDRESS, buff_rx, 1);
#endif
		//i2c_read_data(0x27,buff ,1);
		//i2c_send_data(MAX_DEVICE_ADDRESS,buff ,1);
		//i2c_send_and_read(MAX_DEVICE_ADDRESS, 127);
		//delay_ms(1000);
		//max_port_status_check(port_n, detection_str, class_str)
	}

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
	//
	//RTC
	//
	RCC->APB1ENR |= RCC_APB1ENR_PWREN | RCC_APB1ENR_BKPEN;//Power interface clock enable | Backup interface clock enable
	PWR->CR |= PWR_CR_DBP;//
	RCC->BDCR |=RCC_CLOCK_SOURSE_LSE<<RCC_BDCR_RTCSEL_Pos;//тактирование часов от часового кварца
	RCC->BDCR |= RCC_BDCR_LSEON;//External low-speed oscillator enable
	RCC->BDCR |=RCC_BDCR_RTCEN;//включение часов

	while(!(RCC->BDCR & RCC_BDCR_LSERDY)){}//пока не стабилизируется часовой кварц
	//BIT_NUMBER_RESET(PWR->CR, PWR_CR_DBP);
}

void interrupts_first_init(){
	NVIC_EnableIRQ(USART2_IRQn);

	NVIC_EnableIRQ(DMA1_Channel6_IRQn);
	NVIC_EnableIRQ(DMA1_Channel7_IRQn);

	NVIC_EnableIRQ(TIM2_IRQn);
	NVIC_EnableIRQ(TIM3_IRQn);
	 __enable_irq (); // Разрешить прерывания IRQ
	 //__disable_irq ();// Запретить прерывания IRQ
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
