#include "max5980.h"

const char dev_err[]="No answer from device\n\0";
const char dev_err2[]="Probably device is disconnected\n\0";


const char det_NONE[]="Detection status unknown\n\0";
const char det_DCP[]="Positive DC supply connected at the port\n\0";
const char det_HIGH_CAP[]="High capacitance at the port\n\0";
const char det_RLOW[]="Low resistance at the port\n\0";
const char det_DET_OK[]="Detection pass\n\0";
const char det_RHIGH[]="High resistance at the port\n\0";
const char det_OPEN[]="Open port\n\0";
const char det_DCN[]="Low impedance the port\n\0";

const char *port_detection[] = {det_NONE,det_DCP,det_HIGH_CAP,det_RLOW,det_DET_OK,det_RHIGH,det_OPEN,det_DCN};
//
const char class_Unknown[]="Class Unknown\n\0";
const char class_1[]="Class 1\n\0";
const char class_2[]="Class 2\n\0";
const char class_3[]="Class 3\n\0";
const char class_4[]="Class 4\n\0";
const char class_5[]="Class 5\n\0";
const char class_0[]="Class 0\n\0";
const char class_CURR_LIM[]="Current limit\n\0";

const char *port_class[] = {class_Unknown,class_1,class_2,class_3,class_4,class_5,class_0,class_CURR_LIM};
//
#define MAX_CLASS_MASK 7
#define MAX_CLASS_OFFSET 0

#define MAX_DETECTION_MASK 7
#define MAX_DETECTION_OFFSET 4

void max_port_status_check(uint8_t port_n,const char** detection_str, const char** class_str){
	uint8_t port_status_reg = 0;
	uint8_t class = 0, detection = 0;
	if(port_n>= MAX_PORT_NUMBER){//if incorrect input data
		return;
	}
	port_status_reg = i2c_send_and_read(MAX_DEVICE_ADDRESS, MAX_REG_STATUS_0+port_n);
	//no answer
	if(port_status_reg <0){
		*class_str  = dev_err;
		*detection_str = dev_err2;
		return;
	}
	//if correct data
	detection =( port_status_reg & (MAX_DETECTION_MASK<<MAX_DETECTION_OFFSET) )>>MAX_DETECTION_OFFSET;
	class = (port_status_reg & (MAX_CLASS_MASK<<MAX_CLASS_OFFSET) )>>MAX_CLASS_OFFSET;
	*class_str  = port_class[class];
	*detection_str = port_detection[detection];
}
