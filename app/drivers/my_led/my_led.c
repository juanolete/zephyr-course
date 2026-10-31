#include "zephyr/devicetree.h"
#include <zephyr/logging/log.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/gpio.h>

#define DT_DRV_COMPAT my_led

LOG_MODULE_REGISTER(my_led, LOG_LEVEL_INF);

/* "driver_led" alias defined in app.overlay file. */
#define LED_HEARTBEAT   DT_ALIAS(driver_led)

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_HEARTBEAT, gpios);

// Turn On LED
int sensor_sample_fetch_my_impl(const struct device *dev, enum sensor_channel chan) {
  	LOG_INF("From Sensor Sample Fetch, LED turn On");

	return gpio_pin_set_dt(&led, 1);
};

// Turn Off LED
int channel_get_my_impl(const struct device *dev, enum sensor_channel chan,
                        struct sensor_value *val) {
  	LOG_INF("From Channel Get, LED turn Off");
	
	return gpio_pin_set_dt(&led, 0);
};

static DEVICE_API(sensor, my_api) = {
    .channel_get = channel_get_my_impl,
    .sample_fetch = sensor_sample_fetch_my_impl,
};

// Init fn
static int init(const struct device *dev) {
	LOG_INF("Initializing LED driver");

	if (!gpio_is_ready_dt(&led)) return 0;
	LOG_INF("GPIO ready");
	
    if (gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE) < 0) return 0;
	LOG_INF("GPIO configured");
	
	LOG_INF("Device initialized!");
  	return 0;
}

#define DEV_INST(inst) DEVICE_DT_INST_DEFINE(inst, init, NULL, NULL, NULL, POST_KERNEL, 80, &my_api);

DT_INST_FOREACH_STATUS_OKAY(DEV_INST);