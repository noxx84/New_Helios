/*
 * tect.c
 *
 *  Created on: 02.09.2015
 *      Author: X-SzczyptaM
 */

/*
 * Adaptation of Paul Stoffregen's One wire library to the ESP8266 and
 * Necromant's Frankenstein firmware by Erland Lewin <erland@lewin.nu>
 *
 * Paul's original library site:
 *   http://www.pjrc.com/teensy/td_libs_OneWire.html
 *
 * See also http://playground.arduino.cc/Learning/OneWire
 *
 */
#include "espmissingincludes.h"
#include "ets_sys.h"
#include "os_type.h"
#include "mem.h"
#include "osapi.h"
#include "user_interface.h"

#include "espconn.h"
#include "gpio.h"
#include "driver/uart.h"

#include "pin_map.h"

#include "ds1820.h"
#include <stdlib.h>
//s#include <generic/macros.h>

/*
 * static function prototypes
 */
static int ds_search( OneWireParam* pParam ,char *newAddr );
static void reset_search(OneWireParam* pParam);
static void write_bit( int v );
static void write( char v, int parasitePower );
static inline int read_bit(void);
static inline void write_bit( int v );
static void select(const char *rom);
void ds_init( int gpio );
static char reset( int gpioPin);
static char read();
static char crc8(const char *addr, char len);
static void set_ds18b20_ready_flag(void);
int ICACHE_FLASH_ATTR  cb_timer_tHelios_Operate(void);
ETSTimer OneWireTimer;
ETSTimer HeliosOperate;
char ready_flag;

OneWireParam AppParams;
ControllerConfig HeliosDriverConfig = {
		{
		{
			"BOILER_DOWN",
			DS_DUMMY,
			SENSOR_BOILER_DOWN
		},
		{
			"BOILER_MIDDLE",
			DS_DUMMY,
			SENSOR_BOILER_DOWN
		},
		{
			"BOILER_UP",
			DS_DUMMY,
			SENSOR_BOILER_DOWN
		},
		{
			"SENSOR_STOVE",
			DS_DUMMY,
			SENSOR_STOVE
		},
		},
		{
			{
				"VALVE",
				1,
				10,
				VALVE_CONTROL_GPIO,
				0,
			},
			{
				"POMP",
				1,
				10,
				POMP_CONTROL_GPIO,
				0,
			}
		},
		0,
		ONE_WIRE_GPIO,
};


OneWireParam *
ICACHE_FLASH_ATTR pOneWireParam_Descriptor(void) {
	return &AppParams;
}
ControllerConfig *
ICACHE_FLASH_ATTR pControllerConfig_Descriptor(void) {
	return &HeliosDriverConfig;
}

int ICACHE_FLASH_ATTR
set_Point_ds18b20(int set_point_A , int set_point_B ,char ctrl_elemnt) {
	OneWireParam* pParam = &AppParams;
	ControllerConfig *pApp = &HeliosDriverConfig;

	pApp->ControlElemnt[ctrl_elemnt].u8HysteresisParamOff = set_point_B;
	pApp->ControlElemnt[ctrl_elemnt].u8HysteresisParamOn = set_point_A;

	return 1;
}

int ICACHE_FLASH_ATTR
initHelios_Operate(void) {

	os_timer_disarm(&HeliosOperate);
	os_timer_setfn(&HeliosOperate, (os_timer_func_t *) cb_timer_tHelios_Operate,
			NULL );
	os_timer_arm(&HeliosOperate, 5000, 1);

}
/**
 *
 */
int ICACHE_FLASH_ATTR
cb_timer_tHelios_Operate(void) {
	ControllerConfig *pApp = &HeliosDriverConfig;
	OneWireParam* pParam = &AppParams;
	Temperature temp;
	float midd_temp;
	float solar_temp;
	int i;

	if (pApp->bConfigDoneFlag) {
		for (i = 0; i < SENSOR_COUT - 1; i++) {
			pApp->SensorSet[i].read_out = read_temp_ds18b20(
					pApp->SensorSet[i].sensor_name);
			midd_temp += ((((pApp->SensorSet[i].read_out.tVal * 100)
					+ pApp->SensorSet[i].read_out.tFract)) / 100);
		}
		i++;
		pApp->SensorSet[i].read_out = read_temp_ds18b20(
				pApp->SensorSet[i].sensor_name);
		solar_temp = ((((pApp->SensorSet[i].read_out.tVal * 100)
				+ pApp->SensorSet[i].read_out.tFract)) / 100);

		if (!pApp->ControlElemnt[POMP_CONTROL_GPIO].uIManualCtrl) {
			if (!GPIO_INPUT_GET(
					pApp->ControlElemnt[POMP_CONTROL_GPIO].gpio)) {
				if (midd_temp
						+ pApp->ControlElemnt[POMP_CONTROL_GPIO].u8HysteresisParamOn
						< solar_temp) {
					GPIO_OUTPUT_SET(pApp->ControlElemnt[POMP_CONTROL_GPIO].gpio,
							1);
				}

			} else {
				if (midd_temp
						+ pApp->ControlElemnt[POMP_CONTROL_GPIO].u8HysteresisParamOff
						> solar_temp) {
					GPIO_OUTPUT_SET(pApp->ControlElemnt[POMP_CONTROL_GPIO].gpio,
							0);
				}
			}
			if (!pApp->ControlElemnt[VALVE_CONTROL_GPIO].uIManualCtrl) {
				if (!GPIO_INPUT_GET(
						pApp->ControlElemnt[VALVE_CONTROL_GPIO].gpio)) {
					if (midd_temp
							+ pApp->ControlElemnt[VALVE_CONTROL_GPIO].u8HysteresisParamOn
							< solar_temp) {
						GPIO_OUTPUT_SET(
								pApp->ControlElemnt[VALVE_CONTROL_GPIO].gpio,
								1);
					}

				} else {
					if (midd_temp
							+ pApp->ControlElemnt[VALVE_CONTROL_GPIO].u8HysteresisParamOff
							> solar_temp) {
						GPIO_OUTPUT_SET(
								pApp->ControlElemnt[VALVE_CONTROL_GPIO].gpio,
								0);
					}

				}
			}
		}
	}
}

/**
 *
 */
int ICACHE_FLASH_ATTR
cb_mqtt_broatcast_tHelios_Operate(void) {
	/* To be done */
}


/**
 *
 */
int ICACHE_FLASH_ATTR
cb_mqtt_subscribe_tHelios_Operate(void) {
	/* To be done */
}

int ICACHE_FLASH_ATTR
init_ds18b20(void) {
	ControllerConfig *pApp = &HeliosDriverConfig;

	if (!is_valid_gpio_pin(pApp->OneWirePIN)) {
		os_printf("Invalid GPIO pin number.\n");
		return false;
	}

	ds_init(pApp->OneWirePIN);
	os_printf("After init.\n");

	reset(pApp->OneWirePIN);
	write(DS1820_SKIP_ROM, 1);
	write(DS1820_CONVERT_T, 1);

	//750ms 1x, 375ms 0.5x, 188ms 0.25x, 94ms 0.12x
	os_timer_disarm(&OneWireTimer);
	os_timer_setfn(&OneWireTimer, (os_timer_func_t *)set_ds18b20_ready_flag, NULL);
	os_timer_arm(&OneWireTimer, 750, 0);
	//os_delay_us(750 * 1000);
}

int ICACHE_FLASH_ATTR
search_ds18b20(char ds_position) {
	OneWireParam* pParam= &AppParams;
	ControllerConfig *pApp = &HeliosDriverConfig;
	int r;
	addr_ds1820 addr;
	r = ds_search(pParam,addr);
	if (r) {
		os_printf("Found Device @ %02x %02x %02x %02x %02x %02x %02x %02x\n",
				addr[0], addr[1], addr[2], addr[3], addr[4], addr[5], addr[6],
				addr[7]);
		if (crc8(addr, 7) != addr[7])
			os_printf("CRC mismatch, crc=%xd, addr[7]=%xd\n", crc8(addr, 7),
					addr[7]);
		else {
			memcpy(pApp->SensorSet[ds_position].temp_sensor,addr,siezeof(addr));
			os_printf("Device is added to list.\n");
		}

		switch (addr[0]) {
		case DS18S20:
			os_printf("Device is DS18S20 family.\n");
			break;

		case DS18B20:
			os_printf("Device is DS18B20 family.\n");
			break;

		default:
			os_printf("Device is unknown family.\n");
			return 1;
		}
	}
	else
		os_printf("No device found.\n");

	return r;
}

Temperature ICACHE_FLASH_ATTR
read_temp_ds18b20(const char* const* argv) {
	Temperature temp;
	OneWireParam* pParam = &AppParams;
	ControllerConfig *pApp = &HeliosDriverConfig;
	int r, ds_position;
	const char *tmp = argv[1];
	ds_position = skip_atoi(&tmp);
	addr_ds1820 addr;
	char data[12];
	int i;

	reset(pApp->OneWirePIN);
	select(pApp->SensorSet[ds_position].temp_sensor);
	write(DS1820_READ_SCRATCHPAD, 0);

	for (i = 0; i < 9; i++) {
		data[i] = read();
		os_printf("%2x ", data[i]);
	}
	os_printf("\n");

	// float arithmetic isn't really necessary, tVal and tFract are in 1/10 �C;

	temp.tVal = (data[1] << 8) | data[0];
	if (temp.tVal & 0x8000) {
		temp.tVal = (temp.tVal ^ 0xffff) + 1;				// 2's complement
		temp.tSign = '-';
	} else
		temp.tSign = '+';

	// datasize differs between DS18S20 and DS18B20 - 9bit vs 12bit
	if (addr[0] == DS18S20) {
		temp.tFract = (temp.tVal & 0x01) ? 50 : 0;		// 1bit Fract for DS18S20
		temp.tVal >>= 1;
	} else {
		temp.tFract = (temp.tVal & 0x0f) * 100 / 16;		// 4bit Fract for DS18B20
		temp.tVal >>= 4;
	}
	os_printf("Temperature: %c%d.%02d �C\n", temp.tSign, temp.tVal, temp.tFract);
	return temp;
}

static void set_ds18b20_ready_flag(void) {
	OneWireParam* pParam = &AppParams;
	pParam->uIReady_flag=TRUE;
	reset_search(pParam);
}
/*
 * Parameter: <gpio>
 */
/*
static int do_ds18b20(int argc, const char* const* argv)
{
	const char *tmp = argv[1];
	int gpio; // = skip_atoi(&tmp);
	int r, i;
	char addr[8], data[12];

	gpio = skip_atoi( &tmp );
	bool getall = ((argc >= 3) && (strcmp(argv[2], "all") == 0));

	// We will need to use microsecond timer
	// wait 500 us
	// system_timer_reinit();

	if(!is_valid_gpio_pin(gpio)){
		os_printf( "Invalid GPIO pin number.\n" );
		return false;
	}

	ds_init( gpio );
	os_printf( "After init.\n" );

	reset();
	write( DS1820_SKIP_ROM, 1 );
	write( DS1820_CONVERT_T, 1 );

	//750ms 1x, 375ms 0.5x, 188ms 0.25x, 94ms 0.12x
	os_delay_us( 750*1000 );
	wdt_feed();

	reset_search();
	do{
		r = ds_search( addr );
		if( r )
		{
			os_printf( "Found Device @ %02x %02x %02x %02x %02x %02x %02x %02x\n",
					addr[0], addr[1], addr[2], addr[3], addr[4], addr[5], addr[6], addr[7] );
			if( crc8( addr, 7 ) != addr[7] )
				os_printf( "CRC mismatch, crc=%xd, addr[7]=%xd\n", crc8( addr, 7 ), addr[7] );

			switch( addr[0] )
			{
				case DS18S20:
					os_printf( "Device is DS18S20 family.\n" );
					break;

				case DS18B20:
					os_printf( "Device is DS18B20 family.\n" );
					break;

				default:
					os_printf( "Device is unknown family.\n" );
					return 1;
			}
		}
		else {
			if(!getall){
				console_printf( "No DS18x20 detected, sorry\n" );
				return 1;
			} else break;
		}

		os_printf( "Scratchpad: " );
		reset();
		select( addr );
		write( DS1820_READ_SCRATCHPAD, 0 );

		for( i = 0; i < 9; i++ )
		{
			data[i] = read();
			dbg( "%2x ", data[i] );
		}
		os_printf( "\n" );

		// float arithmetic isn't really necessary, tVal and tFract are in 1/10 �C
		uint16_t tVal, tFract;
		char tSign;

		tVal = (data[1] << 8) | data[0];
		if (tVal & 0x8000) {
			tVal = (tVal ^ 0xffff) + 1;				// 2's complement
			tSign = '-';
		} else
			tSign = '+';

		// datasize differs between DS18S20 and DS18B20 - 9bit vs 12bit
		if (addr[0] == DS18S20) {
			tFract = (tVal & 0x01) ? 50 : 0;		// 1bit Fract for DS18S20
			tVal >>= 1;
		} else {
			tFract = (tVal & 0x0f) * 100 / 16;		// 4bit Fract for DS18B20
			tVal >>= 4;
		}

		if(getall){
			os_printf( "%02x%02x%02x%02x%02x%02x%02x%02x %c%d.%02d\n",
				addr[0], addr[1], addr[2], addr[3], addr[4], addr[5], addr[6], addr[7],
				tSign, tVal, tFract);
		}else{
			os_printf( "Temperature: %c%d.%02d �C\n",
				tSign, tVal, tFract);
			return r;
		}

	} while(getall);

	return r;
}
*/



void ds_init( int gpio )
{
	//set gpio2 as gpio pin
	PIN_FUNC_SELECT(pin_mux[gpio], pin_func[gpio]);

	//disable pulldown
	PIN_PULLDWN_DIS(pin_mux[gpio]);

	//enable pull up R
	PIN_PULLUP_EN(pin_mux[gpio]);

	// Configure the GPIO with internal pull-up
	// PIN_PULLUP_EN( gpio );

	GPIO_DIS_OUTPUT( gpio );

	//gpioPin = gpio;
}
/**
 *
 */
static void reset_search(OneWireParam* pParam)
{
	// reset the search state
	pParam->LastDiscrepancy = 0;
	pParam->LastDeviceFlag = FALSE;
	pParam->LastFamilyDiscrepancy = 0;
	for(int i = 7; ; i--) {
		pParam->ROM_NO[i] = 0;
		if ( i == 0) break;
	}
}

// Perform the onewire reset function.  We will wait up to 250uS for
// the bus to come high, if it doesn't then it is broken or shorted
// and we return a 0;
//
// Returns 1 if a device asserted a presence pulse, 0 otherwise.
//
static char reset( int gpioPin)
{
	//	IO_REG_TYPE mask = bitmask;
	//	volatile IO_REG_TYPE *reg IO_REG_ASM = baseReg;
	int r;
	char retries = 125;

	// noInterrupts();
	// DIRECT_MODE_INPUT(reg, mask);
	GPIO_DIS_OUTPUT( gpioPin );

	// interrupts();
	// wait until the wire is high... just in case
	do {
		if (--retries == 0) return 0;
		os_delay_us(2);
	} while ( !GPIO_INPUT_GET( gpioPin ));

	// noInterrupts();
	GPIO_OUTPUT_SET( gpioPin, 0 );
	// DIRECT_WRITE_LOW(reg, mask);
	// DIRECT_MODE_OUTPUT(reg, mask);	// drive output low
	// interrupts();
	os_delay_us(480);
	// noInterrupts();
	GPIO_DIS_OUTPUT( gpioPin );
	// DIRECT_MODE_INPUT(reg, mask);	// allow it to float
	os_delay_us(70);
	// r = !DIRECT_READ(reg, mask);
	r = !GPIO_INPUT_GET( gpioPin );
	// interrupts();
	os_delay_us(410);

	return r;
}

/* pass array of 8 bytes in */
static int ds_search( OneWireParam* pParam ,char *newAddr )
{
	char id_bit_number;
	char last_zero, rom_byte_number;
	char id_bit, cmp_id_bit;
	int search_result;
	ControllerConfig *pApp = &HeliosDriverConfig;
	unsigned char rom_byte_mask, search_direction;

	// initialize for search
	id_bit_number = 1;
	last_zero = 0;
	rom_byte_number = 0;
	rom_byte_mask = 1;
	search_result = 0;

	// if the last call was not the last one
	if (!pParam->LastDeviceFlag)
	{
		// 1-Wire reset
		if (!reset(pApp->OneWirePIN))
		{
			// reset the search
			pParam->LastDiscrepancy = 0;
			pParam->LastDeviceFlag = FALSE;
			pParam->LastFamilyDiscrepancy = 0;
			return FALSE;
		}

		// issue the search command
		write(DS1820_SEARCHROM, 0);

		// loop to do the search
		do
		{
			// read a bit and its complement
			id_bit = read_bit();
			cmp_id_bit = read_bit();

			// check for no devices on 1-wire
			if ((id_bit == 1) && (cmp_id_bit == 1))
				break;
			else
			{
				// all devices coupled have 0 or 1
				if (id_bit != cmp_id_bit)
					search_direction = id_bit;  // bit write value for search
				else
				{
					// if this discrepancy if before the Last Discrepancy
					// on a previous next then pick the same as last time
					if (id_bit_number < pParam->LastDiscrepancy)
						search_direction = ((pParam->ROM_NO[rom_byte_number] & rom_byte_mask) > 0);
					else
						// if equal to last pick 1, if not then pick 0
						search_direction = (id_bit_number == pParam->LastDiscrepancy);

					// if 0 was picked then record its position in LastZero
					if (search_direction == 0)
					{
						last_zero = id_bit_number;

						// check for Last discrepancy in family
						if (last_zero < 9)
							pParam->LastFamilyDiscrepancy = last_zero;
					}
				}

				// set or clear the bit in the ROM byte rom_byte_number
				// with mask rom_byte_mask
				if (search_direction == 1)
					pParam->ROM_NO[rom_byte_number] |= rom_byte_mask;
				else
					pParam->ROM_NO[rom_byte_number] &= ~rom_byte_mask;

				// serial number search direction write bit
				write_bit(search_direction);

				// increment the byte counter id_bit_number
				// and shift the mask rom_byte_mask
				id_bit_number++;
				rom_byte_mask <<= 1;

				// if the mask is 0 then go to new SerialNum byte rom_byte_number and reset mask
				if (rom_byte_mask == 0)
				{
					rom_byte_number++;
					rom_byte_mask = 1;
				}
			}
		}
		while(rom_byte_number < 8);  // loop until through all ROM bytes 0-7

		// if the search was successful then
		if (!(id_bit_number < 65))
		{
			// search successful so set LastDiscrepancy,LastDeviceFlag,search_result
			pParam->LastDiscrepancy = last_zero;

			// check for last device
			if (pParam->LastDiscrepancy == 0)
				pParam->LastDeviceFlag = TRUE;

			search_result = TRUE;
		}
	}

	// if no device found then reset counters so next 'search' will be like a first
	if (!search_result || !pParam->ROM_NO[0])
	{
		pParam->LastDiscrepancy = 0;
		pParam->LastDeviceFlag = FALSE;
		pParam->LastFamilyDiscrepancy = 0;
		search_result = FALSE;
	}
	for (int i = 0; i < 8; i++) newAddr[i] = pParam->ROM_NO[i];
	return search_result;
}

//
// Write a byte. The writing code uses the active drivers to raise the
// pin high, if you need power after the write (e.g. DS18S20 in
// parasite power mode) then set 'power' to 1, otherwise the pin will
// go tri-state at the end of the write to avoid heating in a short or
// other mishap.
//
static void write( char v, int power ) {
	char bitMask;
	ControllerConfig *pApp = &HeliosDriverConfig;
	for (bitMask = 0x01; bitMask; bitMask <<= 1) {
		write_bit( (bitMask & v)?1:0);
	}
	if ( !power) {
		// noInterrupts();
		GPIO_DIS_OUTPUT( pApp->OneWirePIN);
		GPIO_OUTPUT_SET( pApp->OneWirePIN, 0 );
		// DIRECT_MODE_INPUT(baseReg, bitmask);
		// DIRECT_WRITE_LOW(baseReg, bitmask);
		// interrupts();
	}
}

//
// Write a bit. Port and bit is used to cut lookup time and provide
// more certain timing.
//
static inline void write_bit( int v )
{
	// IO_REG_TYPE mask=bitmask;
	//	volatile IO_REG_TYPE *reg IO_REG_ASM = baseReg;
	ControllerConfig *pApp = &HeliosDriverConfig;
	GPIO_OUTPUT_SET( pApp->OneWirePIN, 0 );
	if( v ) {
		// noInterrupts();
		//	DIRECT_WRITE_LOW(reg, mask);
		//	DIRECT_MODE_OUTPUT(reg, mask);	// drive output low
		os_delay_us(10);
		GPIO_OUTPUT_SET( pApp->OneWirePIN, 1 );
		// DIRECT_WRITE_HIGH(reg, mask);	// drive output high
		// interrupts();
		os_delay_us(55);
	} else {
		// noInterrupts();
		//	DIRECT_WRITE_LOW(reg, mask);
		//	DIRECT_MODE_OUTPUT(reg, mask);	// drive output low
		os_delay_us(65);
		GPIO_OUTPUT_SET( pApp->OneWirePIN, 1 );
		//	DIRECT_WRITE_HIGH(reg, mask);	// drive output high
		//		interrupts();
		os_delay_us(5);
	}
}

//
// Read a bit. Port and bit is used to cut lookup time and provide
// more certain timing.
//
static inline int read_bit(void)
{
	//IO_REG_TYPE mask=bitmask;
	//volatile IO_REG_TYPE *reg IO_REG_ASM = baseReg;
	int r;
	ControllerConfig *pApp = &HeliosDriverConfig;
	// noInterrupts();
	GPIO_OUTPUT_SET( pApp->OneWirePIN, 0 );
	// DIRECT_MODE_OUTPUT(reg, mask);
	// DIRECT_WRITE_LOW(reg, mask);
	os_delay_us(3);
	GPIO_DIS_OUTPUT( pApp->OneWirePIN );
	// DIRECT_MODE_INPUT(reg, mask);	// let pin float, pull up will raise
	os_delay_us(10);
	// r = DIRECT_READ(reg, mask);
	r = GPIO_INPUT_GET( pApp->OneWirePIN );
	// interrupts();
	os_delay_us(53);

	return r;
}

//
// Do a ROM select
//
static void select(const char *rom)
{
	char i;

	write(DS1820_MATCHROM, 0);           // Choose ROM

	for (i = 0; i < 8; i++) write(rom[i], 0);
}

//
// Read a byte
//
static char read() {
	char bitMask;
	char r = 0;

	for (bitMask = 0x01; bitMask; bitMask <<= 1) {
		if ( read_bit()) r |= bitMask;
	}
	return r;
}

//
// Compute a Dallas Semiconductor 8 bit CRC directly.
// this is much slower, but much smaller, than the lookup table.
//
static char crc8(const char *addr, char len)
{
	char crc = 0;

	while (len--) {
		char inbyte = *addr++;
		for (char i = 8; i; i--) {
			char mix = (crc ^ inbyte) & 0x01;
			crc >>= 1;
			if (mix) crc ^= 0x8C;
			inbyte >>= 1;
		}
	}
	return crc;
}


int main() {
	return 0;
}

