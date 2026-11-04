#ifndef MY_LED_H_
#define MY_LED_H_

#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>

#ifdef __cplusplus
extern "C" {
#endif

int my_led_set_gpio(const struct device *dev, const struct gpio_dt_spec *led);

#ifdef __cplusplus
}
#endif

#endif /* MY_LED_H_ */