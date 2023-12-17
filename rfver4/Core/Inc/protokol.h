#ifndef PROTOKOL_H
#define PROTOKOL_H


#include "directive.h"
#include "cc1101.h"
#include "tim.h"//ИСПОЛЬЗУЕТ TIMER 3

#define TIM3_IS_BUSY

#define PROTOCOL_MASTER_ADDRESS			1
#define PROTOCOL_SLAVE_ADDRESS			2
#define PROTOCOL_MAX_PACKET_NUMBER		16

#define PROTOCOL_CRC4_DIVIDER 			0x85


#define PROTOKOL_BYTE_HEADER			0
#define PROTOKOL_BYTE_CONTROL			1
#define PROTOKOL_BYTE_CMD_DATA			2
#define PROTOKOL_BYTE_RSSI				3
//header
#define PROTOKOL_ADRESS_MASTER_MSK		0xC0
#define PROTOKOL_ADRESS_MASTER_OFFSET	0x06
#define PROTOKOL_ADRESS_SLAVE_MSK		0x30
#define PROTOKOL_ADRESS_SLAVE_OFFSET	0x04
#define PROTOKOL_PACKET_NUMBER_MSK		0x0F
#define PROTOKOL_PACKET_NUMBER_OFFSET	0x00


//control
#define PROTOKOL_PREVIOUS_PACKET_CRC_MSK		0xF0
#define PROTOKOL_PREVIOUS_PACKET_CRC_OFFSET		0x04
#define PROTOKOL_PREVIOUS_PACKET_NUMBER_MSK		0x0F
#define PROTOKOL_PREVIOUS_PACKET_NUMBER_OFFSET	0x00
//cmd data
#define PROTOKOL_CMD			 		7
#define PROTOKOL_REPEAT_LAST_MSG 		6
#define PROTOKOL_CMD_PAIRING 			5
#define PROTOKOL_CMD_BREAK_PAIRING 		4
#define PROTOKOL_CMD_ERROR 				3
#define PROTOKOL_CMD_SUCSESS			2
#define PROTOKOL_CMD_FLAG_REPEATING		1
#define PROTOKOL_CMD_MASTER_nSLAVE		0
#define PROTOKOL_DATA_MSK 				0x7f
#define PROTOKOL_DATA_OFFSET 			0x00

struct protocol_format_message_struct;

uint8_t protokol_crc4_generate(uint8_t * data_ptr);
uint8_t protokol_fill_struct(struct protocol_format_message_struct * proto_str, uint8_t * rx_data);

/*
 *
 * пакет состоит из 4байт
 * 	1ый байт хедер
 * 	биты 76 - адрес мастера >0
 * 	биты 54 - адрес раба мб нулем только во время поиска раба
 *  биты 3:0- номер пакета
 * 	2ой байт подтверждение приема предыдузего пакета той стороной
 * 	биты 7:4 - CRC4 по предыдущему пакету
 *  биты 3:0 - адрес предыдущего пакета
 *  3ий байт команда\данные
 * 	бит  7 	 - указывает команда или данные
 *  биты 6:0 - данные
 *  если команда то
 *  бит 6 - повторить отправку последнего пакета
 *  бит 5 - сопряжение
 *  бит 4 - разрыв сопряжения
 *  бит 3 - ошибка
 *  бит 2 - успех
 *  бит 1 - знак повторного сообщения
 *  бит 0 - мастер\не_раб
 *  4ый байт rssi
 *  бит 7:0-rssi
 *  порядок обмена сообщениями
 *  	мастер 	сообщение А-> раб
 *  после получения и расшифровки раб отвечает на сообщение А
 *  	раб 	сообщение Б(с сылкой на сообщение А)-> мастер
 *  если все хорошо мастер осуществляет следущую передачу
 *  	мастер 	сообщение С(с ссылкой на сообщение Б) -> раб
 *  	раб    	сообщение Д(с ссылкой на сообщение С) -> мастер
 *  если сообщение не принято за время гарантированного приема мастер повторяет отправку с знаком повторного сообщения
 *  	мастер 	повтор сообщения С(с ссылкой на сообщение Б) -> раб
 *  	раб 	сообщение Д(с сылкой на сообщение С)-> мастер
 *  если при расшифровке сообщения Д(мастером) окажется что CRC и Номер пакета (пакета С) некоректны, буден произведен пересыл пакета С
 *  с установленным битом ошибки, сигнализирующем о том что раб должен переписать последний принятый пакет и заного его интерпритировать\обработать.
 *  если сообщение принято но в нем есть ошибки
 * мастер после включения отправляет постоянно запрос на сопряжение с 0 адресом получателя.
 * все это время раб слушает эфир молча. при получении запроса раб откликается с битом сопряжения.
 * Когда раб откликается и подтверждает сопряжение, мастер подтверждает сопряжение выставленными битами сопряжение и успех.
 * после этого обмен сообщениями происходит по следущему принципу. Мастер не реже 1 раза в сек отправляет сообщение(даже пустое)
 * после получения сообщения(любого) раб отвечает.
 * В случае, если приема не было в течении максимум 10сек раб считает сопряжение разорванным и готов к новому сопряжению.
 * Если от раба не было ответа в течении максимум 10 секунд, мастер считает соединение разорванным и ищет нового раба.
 * Если раб получил сообщение но не смог расшифровать, раб просит повторить последнее сообщение. Аналогично для мастера. Если процесс повторяется
 * больше 20раз подряд, связь считается разорванной.
 * Если в сообщении оказалась некоректная CRC предыдущего пакета, мастер заного отправляет его с поднятым флагом ошибки и раб его заного должен обработать.
 * Если CRC оказалась некоретной у раба он ответит об этом при следущем получении пакета.
 *
 */


#endif
