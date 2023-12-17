#ifndef STRING_C
#define STRING_C

#include "string.h"
int16_t str_compare(char * str1,char *str2, uint8_t max_lenght){
	uint8_t ui = 0;
	uint16_t result = 0;
	if(max_lenght == 0)
		max_lenght = 255;
	do{
		result = str1[ui] - str2[ui];
		ui++;
	}
	while(!result && str1[ui] != '\0' && str2[ui] != '\0' && ui < max_lenght);
	return result;
}
int16_t str_len(char *str){
	 uint16_t i = 1;
	 while(str[i] != '\0' && i < 255){
		 i++;
	 }
	 if( i >= 255) return -1;
	 return i;
}

uint8_t str_copy(char * str_from, char * str_target, uint8_t char_number){
	uint8_t i;
	if (char_number == 0)
		i = 0xff;
	else
		i = char_number;
	str_target[i] = '\0';
	do{
		i--;
		str_target[i] = str_from[i];
	}while(i);
	return  i;
}

uint8_t str_concatenation(char *target, char *additional_part ){//копируем в target после символа конца строки всю additional_part
	//возвращаем 0 без ошибок 1 ошибка в целевой строке 2 в дополнительной
	uint8_t i,offset;
	uint8_t start_concat = 0;
	for(i = 0; i < 0xfe && target[i] != '\0';i++){}//ищем конец строки
	if(target[i] == '\0') start_concat = 1;
	else
		return 1;//если не нашли выходим
	if(start_concat == 1){
		offset=i;
		for(i=0; i < 0xfe && additional_part[i] != '\0';i++){//до конца строки или до 254
				target[i+offset] = additional_part[i];//копируем
		}
	}
	if(additional_part[i] == '\0'){
		return 2;//если не нашли конец строки additional_part
	}
	else{
		target[i+offset] = '\0';//добавляем символ конца строки
		return 0;
	}
}

void hex32_2str(uint32_t hex, char *str){
	uint8_t i = 0;
	uint8_t j = 0;
	uint8_t middle = 0;
	char ch;
	do{
		str[i] = hex2char(hex & 0xf);
		i++;
		hex = hex >> 4;
	}
	while(hex!= 0);
	str[i] = '\0';
	i--;//в дальнейших операциях не трогаем символ конца строки
	middle = i/2;
	for(;i > middle; i--){//разворачиваем массив задом наперед
		ch = str[i];
		str[i] = str[j];
		str[j] = ch;
		j++;
	}
}

void num32_2str(uint32_t num,uint8_t base, char *str){//число, основание системы счисления, выход
	uint8_t i = 0;
	uint8_t j = 0;
	uint8_t middle = 0;
	uint8_t res;
	char ch;
	do{
		switch(base){
		case 16:
			res = num & 0xf;
			num = num>>4;
			break;
		default:
			res = num % base;
			num /= base;
		}
		str[i] = hex2char(res);
		i++;
	}
	while(num);
	str[i] = '\0';
	i--;//в дальнейших операциях не трогаем символ конца строки
	middle = i/2;
	for(;i > middle; i--){//разворачиваем массив задом наперед
		ch = str[i];
		str[i] = str[j];
		str[j] = ch;
		j++;
	}
}

char hex2char(uint8_t num){//преобразуем 4 бита в 1 hex символ char
	char ch = 0;
	switch(num){
		case 0x0: ch = '0';break;
		case 0x1: ch = '1';break;
		case 0x2: ch = '2';break;
		case 0x3: ch = '3';break;
		case 0x4: ch = '4';break;
		case 0x5: ch = '5';break;
		case 0x6: ch = '6';break;
		case 0x7: ch = '7';break;
		case 0x8: ch = '8';break;
		case 0x9: ch = '9';break;
		case 0xa: ch = 'a';break;
		case 0xb: ch = 'b';break;
		case 0xc: ch = 'c';break;
		case 0xd: ch = 'd';break;
		case 0xe: ch = 'e';break;
		case 0xf: ch = 'f';break;
		default:  ch = 'n';
	}
	return ch;
}
#endif
