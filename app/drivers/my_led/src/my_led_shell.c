#include "zephyr/device.h"
#include <zephyr/shell/shell.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/gpio.h>
#include <string.h>

#include <my_led.h>

#define LED_RED   DT_ALIAS(driver_led)
#define LED_BLUE  DT_ALIAS(driver_led_extra)

const struct gpio_dt_spec led_red = GPIO_DT_SPEC_GET(LED_RED, gpios);
const struct gpio_dt_spec led_blue = GPIO_DT_SPEC_GET(LED_BLUE, gpios);

static int my_driver_fetch_handler(const struct shell *sh, size_t argc, char **argv) {
    shell_info(sh, "My LED Turn ON");
    const struct device *led_driver = shell_device_get_binding("my_led");
    if (led_driver == NULL) {
        shell_error(sh, "Device my_led not found.");
        return 1;
    }
    
    return sensor_sample_fetch_chan(led_driver, SENSOR_CHAN_AMBIENT_TEMP);
}

static int my_driver_channel_get_handler(const struct shell *sh, size_t argc, char **argv) {
    shell_info(sh, "My LED Turn OFF");
    const struct device *led_driver = shell_device_get_binding("my_led");
    if (led_driver == NULL) {
        shell_error(sh, "Device my_led not found.");
        return 1;
    }
    
    struct sensor_value val;
    return sensor_channel_get(led_driver, SENSOR_CHAN_AMBIENT_TEMP, &val);
}

static int my_driver_info_handler(const struct shell *sh, size_t argc, char **argv) {
    const struct device *led_driver = shell_device_get_binding("my_led");
    if (led_driver == NULL) {
        shell_error(sh, "Device my_led not found.");
        return 1;
    }
    
    shell_info(sh, "My LED driver Info. Name=%s, is_ready=%s", 
				"my_led", device_is_ready(led_driver) ? "true" : "false");
    return 0;
}

static int my_driver_set_gpio_info_handler(const struct shell *sh, size_t argc, char **argv) {
    const struct device *led_driver = shell_device_get_binding("my_led");
    if (led_driver == NULL) {
        shell_error(sh, "Device my_led not found.");
        return 1;
    }

	char* set_led = argv[1];

	if (strcmp(set_led, "red") != 0 && strcmp(set_led, "blue")) {
		shell_error(sh, "LED:%s is not valid", argv[1]);
        return 1;
	}
	
	shell_info(sh, "Set LED:%s to led_driver", set_led);
	if (strcmp(set_led, "red") == 0) {
		return my_led_set_gpio(led_driver, &led_red);
    }
	else if (strcmp(set_led, "blue") == 0) {
		return my_led_set_gpio(led_driver, &led_blue);
	}
    return 0;
}

/* Create subcommands array for "sensor" */
SHELL_STATIC_SUBCMD_SET_CREATE(my_led_subcommands,
    SHELL_CMD_ARG(fetch, NULL, "Turn ON LED.", my_driver_fetch_handler, 1, 0),
    SHELL_CMD_ARG(read, NULL, "Turn OFF LED.", my_driver_channel_get_handler, 1, 0),
    SHELL_CMD_ARG(info, NULL, "Get my_led driver info.", my_driver_info_handler, 1, 0),
    SHELL_CMD_ARG(set, NULL, "Set LED GPIO to my_led driver. Can be 'red' or 'blue'.", my_driver_set_gpio_info_handler, 2, 0),
    SHELL_SUBCMD_SET_END
);

/* Register toor command "sensor" with subcommands */
SHELL_CMD_REGISTER(sensor, &my_led_subcommands, "My LED commands.", NULL);
