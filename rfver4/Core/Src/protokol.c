#ifndef PROTOKOL_C
#define PROTOKOL_C

#include "protokol.h"



struct protocol_system_status_struct{
	uint8_t is_paired;
	uint8_t is_master;
	uint8_t master_address;
	uint8_t slave_address;
	uint8_t last_rx_packet_num;
	uint8_t *last_rx_packet_ptr;
	uint8_t last_tx_packet_num;
	uint8_t *last_tx_packet_ptr;
	uint8_t *last_tx_packet_crc4;
	uint8_t is_error;


};

struct protocol_format_message_struct{
	uint8_t master_address;
	uint8_t slave_address;
	uint8_t crc;
	uint8_t this_packet_number;
	uint8_t last_packet_crc;
	uint8_t last_packet_number;
	uint8_t cmd_no_data;
	uint8_t * data_body;
	uint8_t rssi;
	uint8_t cmd_ask_repeat_last_msg;
	uint8_t cmd_pairing;
	uint8_t cmd_break_pairing;
	uint8_t cmd_error;
	uint8_t cmd_sucsess;
	uint8_t cmd_this_msg_is_repeat;
	uint8_t cmd_master_Nslave;
};


int16_t one_tx_with_confirmation(uint8_t *tx_data){
	if(!tx_data) return ERROR_WRONG_ADDRESS;//если нулевой адрес
	//tim3_init()
	return 0;
}












uint8_t protocol_msg_str_init(struct protocol_format_message_struct * msg_str,struct protocol_system_status_struct * main_str, uint8_t is_master){
	msg_str->master_address = main_str->master_address;
	msg_str->master_address = main_str->slave_address;
	msg_str->this_packet_number =(main_str->last_rx_packet_num +1)%PROTOCOL_MAX_PACKET_NUMBER;
	msg_str->last_packet_crc = 0;
	msg_str->last_packet_number = 0;
	msg_str->cmd_no_data = 1;
	msg_str->data_body = 0;
	msg_str->rssi = 0;
	msg_str->cmd_ask_repeat_last_msg = 0;
	msg_str->cmd_pairing = 1;
	msg_str->cmd_break_pairing = 0;
	msg_str->cmd_error = 0;
	msg_str->cmd_sucsess = 0;
	msg_str->cmd_this_msg_is_repeat = 0;
	msg_str->cmd_master_Nslave =is_master;

	return 0;
}
uint8_t protokol_fill_struct(struct protocol_format_message_struct * proto_str, uint8_t * rx_data){
 if(!proto_str || !rx_data)
	 return ERROR_WRONG_ADDRESS;
 proto_str->master_address 			= (rx_data[PROTOKOL_BYTE_HEADER]&PROTOKOL_ADRESS_MASTER_MSK)>>6;
 proto_str->slave_address 			= (rx_data[PROTOKOL_BYTE_HEADER]&PROTOKOL_ADRESS_SLAVE_MSK)>>4;
 proto_str->this_packet_number 		= (rx_data[PROTOKOL_BYTE_HEADER]&PROTOKOL_PACKET_NUMBER_MSK);
 proto_str->last_packet_crc 		= (rx_data[PROTOKOL_BYTE_CONTROL]&PROTOKOL_PREVIOUS_PACKET_CRC_MSK);
 proto_str->last_packet_number 		= (rx_data[PROTOKOL_BYTE_CONTROL]&PROTOKOL_PREVIOUS_PACKET_NUMBER_MSK);
 proto_str->cmd_no_data 			= (rx_data[PROTOKOL_BYTE_CMD_DATA]&(1<<PROTOKOL_CMD));
 proto_str->data_body 				= (rx_data[PROTOKOL_BYTE_CMD_DATA]&PROTOKOL_DATA_MSK);
 proto_str->rssi 					= rx_data[PROTOKOL_BYTE_RSSI];
 proto_str->cmd_ask_repeat_last_msg = (rx_data[PROTOKOL_BYTE_CMD_DATA]& (1<<PROTOKOL_REPEAT_LAST_MSG))>>PROTOKOL_REPEAT_LAST_MSG;
 proto_str->cmd_pairing 			= (rx_data[PROTOKOL_BYTE_CMD_DATA]& (1<<PROTOKOL_CMD_PAIRING))>>PROTOKOL_CMD_PAIRING;
 proto_str->cmd_break_pairing 		= (rx_data[PROTOKOL_BYTE_CMD_DATA]& (1<<PROTOKOL_CMD_BREAK_PAIRING))>>PROTOKOL_CMD_BREAK_PAIRING ;
 proto_str->cmd_error 				= (rx_data[PROTOKOL_BYTE_CMD_DATA]& (1<<PROTOKOL_CMD_ERROR)) >> PROTOKOL_CMD_ERROR;
 proto_str->cmd_sucsess				= (rx_data[PROTOKOL_BYTE_CMD_DATA]& (1<<PROTOKOL_CMD_SUCSESS)) >> PROTOKOL_CMD_SUCSESS;
 proto_str->cmd_this_msg_is_repeat 	= (rx_data[PROTOKOL_BYTE_CMD_DATA]& (1<<PROTOKOL_CMD_FLAG_REPEATING))>>PROTOKOL_CMD_FLAG_REPEATING;
 proto_str->cmd_master_Nslave 		= (rx_data[PROTOKOL_BYTE_CMD_DATA]& (1<<PROTOKOL_CMD_MASTER_nSLAVE))>>PROTOKOL_CMD_MASTER_nSLAVE;
 return 0;
}

uint8_t protokol_fill_data_from_struct(struct protocol_format_message_struct * proto_str, uint8_t * rx_data){
	//получаем сообщение которое можно отправлять.
	uint8_t i;
	if(!proto_str || !rx_data)
		 return ERROR_WRONG_ADDRESS;
	for(i =0; i < CC1101_FIXED_PACKET_LENGTH; i++){
		rx_data[i] = 0;
	}
	rx_data[PROTOKOL_BYTE_HEADER] 		|= proto_str->master_address<<PROTOKOL_ADRESS_MASTER_OFFSET;
	rx_data[PROTOKOL_BYTE_HEADER] 		|= proto_str->slave_address<<PROTOKOL_ADRESS_SLAVE_OFFSET;
	rx_data[PROTOKOL_BYTE_HEADER] 		|= proto_str->this_packet_number<<PROTOKOL_PACKET_NUMBER_OFFSET;
	rx_data[PROTOKOL_BYTE_CONTROL] 		|= proto_str->last_packet_crc << PROTOKOL_PREVIOUS_PACKET_CRC_OFFSET;
	rx_data[PROTOKOL_BYTE_CONTROL] 		|= proto_str->last_packet_number<< PROTOKOL_PREVIOUS_PACKET_NUMBER_OFFSET;
	rx_data[PROTOKOL_BYTE_CMD_DATA] 	|= proto_str->cmd_no_data<<PROTOKOL_CMD;
	if(proto_str->cmd_no_data){
		rx_data[PROTOKOL_BYTE_CMD_DATA] 	|= proto_str->cmd_ask_repeat_last_msg<<PROTOKOL_REPEAT_LAST_MSG;
		rx_data[PROTOKOL_BYTE_CMD_DATA] 	|= proto_str->cmd_pairing<<PROTOKOL_CMD_PAIRING;
		rx_data[PROTOKOL_BYTE_CMD_DATA] 	|= proto_str->cmd_break_pairing<<PROTOKOL_CMD_BREAK_PAIRING;
		rx_data[PROTOKOL_BYTE_CMD_DATA] 	|= proto_str->cmd_error<<PROTOKOL_CMD_ERROR;
		rx_data[PROTOKOL_BYTE_CMD_DATA] 	|= proto_str->cmd_sucsess<<PROTOKOL_CMD_SUCSESS;
		rx_data[PROTOKOL_BYTE_CMD_DATA] 	|= proto_str->cmd_this_msg_is_repeat<<PROTOKOL_CMD_FLAG_REPEATING;
		rx_data[PROTOKOL_BYTE_CMD_DATA] 	|= proto_str->cmd_master_Nslave<<PROTOKOL_CMD_MASTER_nSLAVE;
	}
	else{
		rx_data[PROTOKOL_BYTE_CMD_DATA] |= proto_str->data_body<<PROTOKOL_DATA_OFFSET;
	}
	rx_data[PROTOKOL_BYTE_RSSI] 		|= proto_str->rssi;

	return 0;
}

uint8_t protokol_crc4_generate(uint8_t * data_ptr){
	uint32_t data = 0;
	uint8_t i, crc;
	for(i = 0; i < CC1101_FIXED_PACKET_LENGTH;i++){
		data |= data_ptr[i]<<(i*BYTE_SIZE);
	}
	crc = (data % PROTOCOL_CRC4_DIVIDER)&0x0f;
	return crc;
}

#endif
