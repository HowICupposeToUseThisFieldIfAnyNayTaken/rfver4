#ifndef STRING_H
#define STRING_H
#include "directive.h"

int16_t str_len(char *str);
uint8_t str_concatenation(char *target, char *additional_part);
uint8_t str_copy(char * str_from, char * str_target, uint8_t char_number);
int16_t str_compare(char * str1,char *str2, uint8_t max_lenght);

char hex2char(uint8_t num);
void num32_2str(uint32_t num,uint8_t base, char *str);
void hex32_2str(uint32_t hex, char *str);
#endif
