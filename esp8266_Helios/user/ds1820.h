/*
 * ds1820.h
 *
 *  Created on: 02.09.2015
 *      Author: X-SzczyptaM
 */

#ifndef DS1820_H_
#define DS1820_H_


#define SENSOR_COUT (4)
#define POMP_CONTROL_GPIO (0)
#define VALVE_CONTROL_GPIO (1)
#define ONE_WIRE_GPIO (5)
#define Nr_OFF_Controlled_Elemenst (2)
typedef unsigned char  addr_ds1820[8];
#define DS_DUMMY	{0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF }

#define DS1820_WRITE_SCRATCHPAD	0x4E
#define DS1820_READ_SCRATCHPAD	0xBE
#define DS1820_COPY_SCRATCHPAD	0x48
#define DS1820_READ_EEPROM		0xB8
#define DS1820_READ_PWRSUPPLY	0xB4
#define DS1820_SEARCHROM		0xF0
#define DS1820_SKIP_ROM			0xCC
#define DS1820_READROM			0x33
#define DS1820_MATCHROM			0x55
#define DS1820_ALARMSEARCH		0xEC
#define DS1820_CONVERT_T		0x44

// DS18x20 family codes
#define DS18S20		0x10
#define DS18B20 	0x28

typedef struct STemperature {
	unsigned short tVal;
	unsigned short tFract;
	char tSign;
}Temperature;

/**
 * @brief enum for sensors positions
 */
typedef enum eControllerSensors {
	SENSOR_BOILER_DOWN, SENSOR_BOILER_MIDDLE, SENSOR_BOILER_UP, SENSOR_STOVE,
} ControllerSensors;

/**
 *
 */
typedef struct sControledElement{
	char ctrlname[32];
	char u8HysteresisParamOn;
	char u8HysteresisParamOff;
	char gpio;
	char uIManualCtrl;
}ControledElement;
/**
 *
 */
typedef struct sControllerSensorSetup{
	char sensor_name[32];
	addr_ds1820 temp_sensor;
	ControllerSensors position;
	Temperature read_out;
}ControllerSensorSetup;
/**
 *
 */
typedef struct sControllerConfig {
	ControllerSensorSetup SensorSet[SENSOR_COUT];;
	ControledElement ControlElemnt[Nr_OFF_Controlled_Elemenst];
	char bConfigDoneFlag;
	char OneWirePIN;
}ControllerConfig;

typedef struct sOneWireParam {
	char uIReady_flag;
	addr_ds1820 ROM_NO;
	char LastDiscrepancy;
	char LastFamilyDiscrepancy;
	char LastDeviceFlag;
}OneWireParam;


Temperature  read_temp_ds18b20(const char* const* argv);
int   init_ds18b20(void);
int   search_ds18b20(char ds_position);
OneWireParam *  pOneWireParam_Descriptor(void) ;
ControllerConfig *  pControllerConfig_Descriptor(void);
int set_Point_ds18b20(int set_point_A , int set_point_B ,char ctrl_elemnt);

#endif /* DS1820_H_ */
