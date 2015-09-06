#ifndef USER_IO_H_
#define USER_IO_H_

bool connected_mqtt_cloud;

int blink_counter_lamp;

#define GPIO 15
#define GPIO_MUX PERIPHS_IO_MUX_MTDO_U
#define GPIO_FUNC FUNC_GPIO15

// This was defined in the old SDK.
#ifndef GPIO_OUTPUT_SET
#define GPIO_OUTPUT_SET(gpio_no, bit_value) \
    gpio_output_set(bit_value<<gpio_no, ((~bit_value)&0x01)<<gpio_no, 1<<gpio_no,0)
#endif

void ICACHE_FLASH_ATTR ioLed(int ena);
void ioInit(void);
void blink(void *arg);

#endif /* USER_IO_H_ */
