#ifndef SI4432_REG_H
#define SI4432_REG_H

#define REGx07_xton (1<<0)

typedef enum _RF_REG_MAP
{
	SI4432_REG_Device_Type = 0x00,
	SI4432_REG_Device_Version = 0x01,
	SI4432_REG_Device_Status = 0x02,
	SI4432_REG_Interrupt_Status1 = 0x03,
	SI4432_REG_Interrupt_Status2 = 0x04,
	SI4432_REG_Interrupt_Enable1 = 0x05,
	SI4432_REG_Interrupt_Enable2 = 0x06,
	SI4432_REG_Operating_Function_Control1 = 0x07,
	SI4432_REG_Operating_Function_Control2 = 0x08,
	SI4432_REG_Crystal_Oscillator_Load_Capacitance = 0x09,
	SI4432_REG_Microcontroller_Output_Clock = 0x0A,
	SI4432_REG_GPIO0_Configuration = 0x0B,
	SI4432_REG_GPIO1_Configuration = 0x0C,
	SI4432_REG_GPIO2_Configuration = 0x0D,
	SI4432_REG_IO_Port_Configuration = 0x0E,
	SI4432_REG_ADC_Configuration = 0x0F,
	SI4432_REG_ADC_Sensor_Amplifier_Offset = 0x10,
	SI4432_REG_ADC_Value = 0x11,
	SI4432_REG_Temperature_Sensor_Control = 0x12,
	SI4432_REG_Temperature_Value_Offset = 0x13,
	SI4432_REG_Wake_Up_Timer_Period1 = 0x14,
	SI4432_REG_Wake_Up_Timer_Period2 = 0x15,
	SI4432_REG_Wake_Up_Timer_Period3 = 0x16,
	SI4432_REG_Wake_Up_Timer_Value1 = 0x17,
	SI4432_REG_Wake_Up_Timer_Value2 = 0x18,
	SI4432_REG_Low_Duty_Cycle_Mode_Duration = 0x19,
	SI4432_REG_Low_Battery_Detector_Threshold = 0x1A,
	SI4432_REG_Battery_Voltage_Level = 0x1B,
	SI4432_REG_IF_Filter_Bandwidth = 0x1C,
	SI4432_REG_AFC_Loop_Gearshift_Override = 0x1D,
	SI4432_REG_AFC_Timing_Control = 0x1E,
	SI4432_REG_Clock_Recovery_Gearshift_Override = 0x1F,
	SI4432_REG_Clock_Recovery_Oversampling_Ratio = 0x20,
	SI4432_REG_Clock_Recovery_Offset2 = 0x21,
	SI4432_REG_Clock_Recovery_Offset1 = 0x22,
	SI4432_REG_Clock_Recovery_Offset0 = 0x23,
	SI4432_REG_Clock_Recovery_Timing_Loop_Gain1 = 0x24,
	SI4432_REG_Clock_Recovery_Timing_Loop_Gain0 = 0x25,
	SI4432_REG_Received_Signal_Strength_Indicator = 0x26,
	SI4432_REG_RSSI_Threshold_For_Clear_Channel_Indicator = 0x27,
	SI4432_REG_Antenna_Diversity_Register1 = 0x28,
	SI4432_REG_Antenna_Diversity_Register2 = 0x29,
	SI4432_REG_AFC_Limiter = 0x2a, //only for rev B1, 4431 A0
	SI4432_REG_AFC_Correction_Read = 0x2b, //only for rev B1
	SI4432_REG_OOK_Counter_Value1 = 0x2c,
	SI4432_REG_OOK_Counter_Value2 = 0x2d,
	SI4432_REG_Slicer_Peak_Hold = 0x2e,
	SI4432_REG_Data_Access_Control = 0x30,
	SI4432_REG_EZmac_Status = 0x31,
	SI4432_REG_Header_Control1 = 0x32,
	SI4432_REG_Header_Control2 = 0x33,
	SI4432_REG_Preamble_Length = 0x34,
	SI4432_REG_Preamble_Detection_Control = 0x35,
	SI4432_REG_Sync_Word3 = 0x36,
	SI4432_REG_Sync_Word2 = 0x37,
	SI4432_REG_Sync_Word1 = 0x38,
	SI4432_REG_Sync_Word0 = 0x39,
	SI4432_REG_Transmit_Header3 = 0x3A,
	SI4432_REG_Transmit_Header2 = 0x3B,
	SI4432_REG_Transmit_Header1 = 0x3C,
	SI4432_REG_Transmit_Header0 = 0x3D,
	SI4432_REG_Transmit_Packet_Length = 0x3E,
	SI4432_REG_Check_Header3 = 0x3F,
	SI4432_REG_Check_Header2 = 0x40,
	SI4432_REG_Check_Header1 = 0x41,
	SI4432_REG_Check_Header0 = 0x42,
	SI4432_REG_Header_Enable3 = 0x43,
	SI4432_REG_Header_Enable2 = 0x44,
	SI4432_REG_Header_Enable1 = 0x45,
	SI4432_REG_Header_Enable0 = 0x46,
	SI4432_REG_Received_Header3 = 0x47,
	SI4432_REG_Received_Header2 = 0x48,
	SI4432_REG_Received_Header1 = 0x49,
	SI4432_REG_Received_Header0 = 0x4A,
	SI4432_REG_Received_Packet_Length = 0x4B,
	SI4432_REG_Analog_Test_Bus = 0x50,
	SI4432_REG_Digital_Test_Bus = 0x51,
	SI4432_REG_TX_Ramp_Control = 0x52,
	SI4432_REG_PLL_Tune_Time = 0x53,
	SI4432_REG_PA_Boost = 0x54,
	SI4432_REG_Calibration_Control = 0x55,
	SI4432_REG_Modem_Test = 0x56,
	SI4432_REG_Chargepump_Test = 0x57,
	SI4432_REG_Chargepump_Current_Trimming_Override = 0x58,
	SI4432_REG_Divider_Current_Trimming = 0x59,
	SI4432_REG_VCO_Current_Trimming = 0x5A,
	SI4432_REG_VCO_Calibration_Override = 0x5B,
	SI4432_REG_Synthesizer_Test = 0x5C,
	SI4432_REG_Block_Enable_Override1 = 0x5D,
	SI4432_REG_Block_Enable_Override2 = 0x5E,
	SI4432_REG_Block_Enable_Override3 = 0x5F,
	SI4432_REG_Channel_Filter_Coefficient_Address = 0x60,
	SI4432_REG_Channel_Filter_Coefficient_Value = 0x61,
	SI4432_REG_Crystal_Oscillator_Control_Test = 0x62,
	SI4432_REG_RC_Oscillator_Coarse_Calibration_Override = 0x63,
	SI4432_REG_RC_Oscillator_Fine_Calibration_Override = 0x64,
	SI4432_REG_LDO_Control_Override = 0x65,
	SI4432_REG_LDO_Level_Settings = 0x66,
	SI4432_REG_Deltasigma_ADC_Tuning1 = 0x67,
	SI4432_REG_Deltasigma_ADC_Tuning2 = 0x68,
	SI4432_REG_AGC_Override1 = 0x69,
	SI4432_REG_AGC_Override2 = 0x6A,
	SI4432_REG_GFSK_FIR_Filter_Coefficient_Address = 0x6B,
	SI4432_REG_GFSK_FIR_Filter_Coefficient_Value = 0x6C,
	SI4432_REG_TX_Power = 0x6D,
	SI4432_REG_TX_Data_Rate1 = 0x6E,
	SI4432_REG_TX_Data_Rate0 = 0x6F,
	SI4432_REG_Modulation_Mode_Control1 = 0x70,
	SI4432_REG_Modulation_Mode_Control2 = 0x71,
	SI4432_REG_Frequency_Deviation = 0x72,
	SI4432_REG_Frequency_Offset = 0x73,
	SI4432_REG_Frequency_Channel_Control = 0x74,
	SI4432_REG_Frequency_Band_Select = 0x75,
	SI4432_REG_Nominal_Carrier_Frequency1 = 0x76,
	SI4432_REG_Nominal_Carrier_Frequency0 = 0x77,
	SI4432_REG_Misc_Settings = 0x78,
	SI4432_REG_Frequency_Hopping_Channel_Select = 0x79,
	SI4432_REG_Frequency_Hopping_Step_Size = 0x7A,
	SI4432_REG_Turn_Around_And15p4 = 0x7B,
	SI4432_REG_TX_FIFO_Control1 = 0x7C,
	SI4432_REG_TX_FIFO_Control2 = 0x7D,
	SI4432_REG_RX_FIFO_Control = 0x7E,
	SI4432_REG_FIFO_Access = 0x7F,
} SI4432_REG_MAP;

#endif