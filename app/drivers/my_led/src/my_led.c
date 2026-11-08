#include "my_led.h"

#include <zephyr/logging/log.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/gpio.h>

#define DT_DRV_COMPAT my_led

LOG_MODULE_REGISTER(my_led, LOG_LEVEL_INF);

struct my_led_data {
	const struct gpio_dt_spec *led;
};

static int my_led_init(const struct device *dev) {
	LOG_DBG("LED driver initialized!");
	
  	return 0;
}

int my_led_turn_on(const struct device *dev, enum sensor_channel chan) {
  	LOG_DBG("My LED turn ON");

	struct my_led_data *data = dev->data;
	return gpio_pin_set_dt(data->led, 1);
};

int my_led_turn_off(const struct device *dev, enum sensor_channel chan,
                        struct sensor_value *val) {
  	LOG_DBG("My LED turn OFF");
	
	struct my_led_data *data = dev->data;
	return gpio_pin_set_dt(data->led, 0);
};

int my_led_set_gpio(const struct device *dev, const struct gpio_dt_spec *led) {
	LOG_DBG("My LED set GPIO");

	struct my_led_data *data = dev->data;
	data->led = led;
	return 0;
};

static DEVICE_API(sensor, my_led_api) = {
    .channel_get = my_led_turn_off,
    .sample_fetch = my_led_turn_on,
};                

#define DEFINE_MY_LED(inst) 						\
	static struct my_led_data my_led_data_##inst; 	\
													\
	DEVICE_DT_INST_DEFINE(inst, 					\
						my_led_init, 				\
						NULL, 						\
						&my_led_data_##inst, 		\
						NULL, 						\
						POST_KERNEL, 				\
						80, 						\
						&my_led_api);		

DT_INST_FOREACH_STATUS_OKAY(DEFINE_MY_LED);
