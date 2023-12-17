#ifndef INC_NEO6M_C
#define INC_NEO6M_C

#include "neo6m.h"

uint8_t neo6m_interrupt;

const uint8_t sync_char1 = 0xb5;
const uint8_t sync_char2 = 0x62;
const uint8_t class_type = 0x0D;
const uint8_t msg_id	 = 0x01;
const uint16_t msg_lenght = 0x10;//16 litle endian

int16_t ubx_msg_is_correct(uint8_t * msg_ptr){
	uint8_t is_not_ubx = 0;
	uint8_t is_not_tim_tp = 0;
	if(!(msg_ptr)) return ERROR_WRONG_ADDRESS;
	//0 if difference
	is_not_ubx = (msg_ptr[0] ^ sync_char1) + (msg_ptr[1] ^ sync_char2);
	is_not_tim_tp = (msg_ptr[2] ^ class_type) + (msg_ptr[3] ^ msg_id);
	if(is_not_tim_tp | is_not_ubx) return ERROR_WRONG_PARAMETRS;
	//ubx_tim_tp_decrypt(&msg_ptr[6]);
	return 1;
}

int16_t ubx_tim_tp_decrypt(uint8_t * msg_ptr, struct ubx_tim_tp * str_tim_tp ){
	if(!(msg_ptr || str_tim_tp)) return ERROR_WRONG_ADDRESS;
	uint8_t * payload_ptr = &msg_ptr[6];
	str_tim_tp->time_of_week_ms = ubx_little_endiad_to_normal(payload_ptr,4);
	str_tim_tp->time_of_week_sub_ms = ubx_little_endiad_to_normal(&(payload_ptr[4]),4);
	str_tim_tp->q_err_ps = ubx_little_endiad_to_normal(&(payload_ptr[8]),4);
	str_tim_tp->weeks = ubx_little_endiad_to_normal(&(payload_ptr[12]),2);
	str_tim_tp->bit_mask = payload_ptr[14];
	return 0;
}
int32_t ubx_set_time(struct ubx_tim_tp * str_tim_tp){
	uint32_t tmp_time = 0;
	if(!str_tim_tp ) return ERROR_WRONG_ADDRESS;
	if(!(str_tim_tp->time_of_week_ms && (global_uart2_rx_time > 0)) ) return ERROR_WRONG_PARAMETRS;

	tmp_time = (str_tim_tp->time_of_week_ms /1000);
	global_relative_time = tmp_time  + (global_relative_time -global_uart2_rx_time);
	global_time_is_real = 1;
	return 0;
}
uint32_t ubx_little_endiad_to_normal(uint8_t *lil_end, uint8_t size){
	uint32_t big_end = 0;
 switch (size){
 case 2:
	 big_end = lil_end[1]<<8 | lil_end[0];
 	 break;
 case 4:
	 big_end = lil_end[3]<<24  | lil_end[2]<<16 | lil_end[1]<<8 | lil_end[0];
	 break;
 };
 return big_end;
}
#endif /* INC_NEO6M_C */
