#ifndef CC1101_REG_H
#define CC1101_REG_H
// ------------------------CC1100  CONTROL--------------------------------
#define IOCFG2       	0x00        	// GDO2 output pin configuration
#define IOCFG1       	0x01        	// GDO1 output pin configuration
#define IOCFG0       	0x02        	// GDO0 output pin configuration
#define FIFOTHR     	0x03       		// RX FIFO and TX FIFO thresholds
#define SYNC1        	0x04        	// Sync word, high INT8U
#define SYNC0        	0x05        	// Sync word, low INT8U
#define PKTLEN       	0x06        	// Packet length
#define PKTCTRL1     	0x07       		// Packet automation control
#define PKTCTRL0    	0x08       		// Packet automation control
#define ADDR         	0x61        	// Device address
#define CHANNR       	0x0A        	// Channel number
#define FSCTRL1      	0x0B        	// Frequency synthesizer control
#define FSCTRL0      	0x0C        	// Frequency synthesizer control
#define FREQ2        	0x0D        	// Frequency control word, high INT8U
#define FREQ1        	0x0E        	// Frequency control word, middle INT8U
#define FREQ0        	0x0F        	// Frequency control word, low INT8U
#define MDMCFG4      	0x10          	// Modem configuration
#define MDMCFG3      	0x11        	// Modem configuration
#define MDMCFG2      	0x12        	// Modem configuration
#define MDMCFG1      	0x13        	// Modem configuration
#define MDMCFG0      	0x14        	// Modem configuration
#define DEVIATN      	0x15        	// Modem deviation setting
#define MCSM2        	0x16        	// Main Radio Control State Machine configuration
#define MCSM1        	0x17        	// Main Radio Control State Machine configuration
#define MCSM0        	0x18        	// Main Radio Control State Machine configuration
#define FOCCFG       	0x19        	// Frequency Offset Compensation configuration
#define BSCFG        	0x1A        	// Bit Synchronization configuration
#define AGCCTRL2     	0x1B        	// AGC control
#define AGCCTRL1     	0x1C        	// AGC control
#define AGCCTRL0     	0x1D        	// AGC control
#define WOREVT1      	0x1E        	// High INT8U Event 0 timeout
#define WOREVT0      	0x1F        	// Low INT8U Event 0 timeout
#define WORCTRL      	0x20        	// Wake On Radio control
#define FREND1       	0x21        	// Front end RX configuration
#define FREND0       	0x22       		// Front end TX configuration
#define FSCAL3       	0x23        	// Frequency synthesizer calibration
#define FSCAL2       	0x24        	// Frequency synthesizer calibration
#define FSCAL1       	0x25        	// Frequency synthesizer calibration
#define FSCAL0       	0x26        	// Frequency synthesizer calibration
#define RCCTRL1      	0x27        	// RC oscillator configuration
#define RCCTRL0      	0x28        	// RC oscillator configuration
#define FSTEST       	0x29        	// Frequency synthesizer calibration control
#define PTEST        	0x2A        	// Production test
#define AGCTEST      	0x2B        	// AGC test
#define TEST2        	0x2C        	// Various test settings
#define TEST1        	0x2D        	// Various test settings
#define TEST0        	0x2E        	// Various test settings

// ---------------STROBE commands--WriteOnly-----------------------------------
#define CC1101_STROBE_SRES         0x30			// Reset chip.
#define CC1101_STROBE_SFSTXON      0x31			// 	Enable and calibrate frequency synthesizer (if MCSM0.FS_AUTOCAL=1).
#define CC1101_STROBE_SXOFF        0x32			// 	Turn off crystal oscillator.
#define CC1101_STROBE_SCAL         0x33			// 	Calibrate frequency synthesizer and turn it off		(enables quick start).
#define CC1101_STROBE_SRX          0x34			// 	Enable RX. Perform calibration first if coming from IDLE and MCSM0.FS_AUTOCAL=1.
#define CC1101_STROBE_STX          0x35			// 	In IDLE state: Enable TX. Perform calibration first if MCSM0.FS_AUTOCAL=1. If in RX state and CCA is enabled:	Only go to TX if channel is clear.
#define CC1101_STROBE_SIDLE        0x36			//	Exit RX / TX, turn off frequency synthesizer and exit Wake-On-Radio mode if applicable.
#define CC1101_STROBE_SAFC         0x37        	//	Perform AFC adjustment of the frequency synthesizer
#define CC1101_STROBE_SWOR         0x38        	//	Start automatic RX polling sequence (Wake-on-Radio)
#define CC1101_STROBE_SPWD         0x39        	//	Enter power down mode when CSn goes high.
#define CC1101_STROBE_SFRX         0x3A        	//	Flush the RX FIFO buffer.
#define CC1101_STROBE_SFTX         0x3B        	//	Flush the TX FIFO buffer.
#define CC1101_STROBE_SWORRST      0x3C        	//	Reset real time clock.
#define CC1101_STROBE_SNOP         0x3D        	//	No operation. May be used to pad strobe commands to two	INT8Us for simpler software.

//----------------------STATUS REGSITER - ReadOnly ------------------------------
#define PARTNUM      0xF0 				//0x30
#define VERSION      0xF1					//0x31
#define FREQEST      0xF2					//0x32
#define LQI          0xF3					//0x33
#define RSSI         0xF4					//0x34
#define MARCSTATE    0xF5					//0x35
#define WORTIME1     0xF6					//0x36
#define WORTIME0     0xF7					//0x37
#define PKTSTATUS    0xF8					//0x38
#define VCO_VC_DAC   0xF9					//0x39

#define TXBYTES      0xFA					//0x3A
#define RXBYTES      0xFB					//0x3B
#define RCCCTRLSTAT1 0xFC
#define RCCCTRLSTAT0 0xFD

#define PATABLE      0xFE
#define RX_TX_FIFO   0xFF


#endif
