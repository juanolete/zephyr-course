#include "zephyr/devicetree.h"
#include <zephyr/logging/log.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/gpio.h>

#define DT_DRV_COMPAT my_led

LOG_MODULE_REGISTER(my_led, LOG_LEVEL_INF);

/* "driver_led" alias defined in app.overlay file. */
#define LED_HEARTBEAT   DT_ALIAS(driver_led)

struct my_led_data {
	struct gpio_dt_spec led;
};

static int my_led_init(const struct device *dev) {
	LOG_INF("Initializing LED driver");

	struct my_led_data *data = dev->data;

	if (!gpio_is_ready_dt(&data->led)) return 0;
	LOG_INF("GPIO ready");
	
    if (gpio_pin_configure_dt(&data->led, GPIO_OUTPUT_ACTIVE) < 0) return 0;
	LOG_INF("GPIO configured");
	
	LOG_INF("Device initialized!");
  	return 0;
}

int my_led_turn_on(const struct device *dev, enum sensor_channel chan) {
  	LOG_INF("My LED turn On");

	struct my_led_data *data = dev->data;
	return gpio_pin_set_dt(&data->led, 1);
};

int my_led_turn_off(const struct device *dev, enum sensor_channel chan,
                        struct sensor_value *val) {
  	LOG_INF("My LED turn Off");
	
	struct my_led_data *data = dev->data;
	return gpio_pin_set_dt(&data->led, 0);
};

static DEVICE_API(sensor, my_led_api) = {
    .channel_get = my_led_turn_off,
    .sample_fetch = my_led_turn_on,
};
                                  	
static struct my_led_data my_led_data = {  					
    .led = GPIO_DT_SPEC_GET(LED_HEARTBEAT, gpios),                          
};  																		

DEVICE_DT_INST_DEFINE(0, my_led_init, NULL, &my_led_data, NULL, POST_KERNEL, 80, &my_led_api);
