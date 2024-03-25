#ifndef INC_MAX5980_H
#define INC_MAX5980_H

#include "directive.h"
#include "i2c.h"
//35 = 010011
#define MAX_DEVICE_ADDRESS 35

#define MAX_ID 0xD0
#define MAX_REG_ID 0x1B
#define MAX_PORT_NUMBER 4
#define MAX_REG_STATUS_0 0x0C
#define MAX_REG_STATUS_1 0x0D
#define MAX_REG_STATUS_2 0x0E
#define MAX_REG_STATUS_3 0x0F

#define MAX_PORT_STATUS_DET_NONE 		0
#define MAX_PORT_STATUS_DET_DCP			1
#define MAX_PORT_STATUS_DET_HIGH_CAP	2
#define MAX_PORT_STATUS_DET_RLOW		3
#define MAX_PORT_STATUS_DET_DET_OK		4
#define MAX_PORT_STATUS_DET_RHIGH		5
#define MAX_PORT_STATUS_DET_OPEN		6
#define MAX_PORT_STATUS_DET_DCN			7

#define MAX_PORT_STATUS_CLASS_UNKNOWN	0
#define MAX_PORT_STATUS_CLASS_1			1
#define MAX_PORT_STATUS_CLASS_2			2
#define MAX_PORT_STATUS_CLASS_3			3
#define MAX_PORT_STATUS_CLASS_4			4
#define MAX_PORT_STATUS_CLASS_5			5
#define MAX_PORT_STATUS_CLASS_0			6
#define MAX_PORT_STATUS_CLASS_I_LIM		7


void max_port_status_check(uint8_t port_n,const char** detection_str, const char** class_str);

#endif /* INC_MAX5980_H_ */
