#ifndef INC_NEO6M_H
#define INC_NEO6M_H

#include "directive.h"
#include "uart.h"
#define NEO6M_UART_NUMBER UART2_NUMBER
#define Neo6M_PPS_PIN PIN_1  //TIMEPULSE PIN
#define Neo6M_PPS_PORT GPIOB  //TIMEPULSE PORT
#define Neo6M_EXTI AFIO_EXTICR1_EXTI1_PB

#define NVIC_NEO6M_PPS_DETECT  EXTI1_IRQn
//ubx format
// b5 62 0d 01 10 00 ***msg*** checksum_byte1 checksum_byte2
//msg format tim-tp
// tow 4bytes, tow_ms 4bytes, q_err_ps 4bytes, weeks 2bytes, bit_mask 1byte, reserved 1byte
// tow0 tow1 tow2 tow3 tow_ms0 tow_ms1 tow_ms2 tow_ms3 tow_ms4 q_err0
//  q_err1 q_err2 q_err3 q_err4 week0 week1 bitmask reserved
struct ubx_tim_tp{
	uint32_t	time_of_week_ms;//Timepulse time of week according to time base
	uint32_t	time_of_week_sub_ms;//Submillisecond part of TOWMS
	int32_t 	q_err_ps;//Quantization error of timepulse
	uint16_t 	weeks;//Timepulse week number according to time base
	uint8_t		bit_mask;//x1 =1  UTC_is_available, x0 -> 0=Time base is GPS, 1= UTC
	uint8_t		reserved;
};

extern uint8_t neo6m_interrupt;
int16_t ubx_msg_is_correct(uint8_t * msg_ptr);
int16_t ubx_tim_tp_decrypt(uint8_t * msg_ptr, struct ubx_tim_tp * str_tim_tp );
uint32_t ubx_little_endiad_to_normal(uint8_t *lil_end, uint8_t size);
int32_t ubx_set_time(struct ubx_tim_tp * str_tim_tp);
#endif /* INC_NEO6M_H */
