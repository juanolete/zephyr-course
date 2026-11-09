#include "zephyr/device.h"
#include <zephyr/shell/shell.h>
#include <zephyr/drivers/sensor.h>

static int my_driver_fetch_handler(const struct shell *sh, size_t argc, char **argv) {
    shell_info(sh, "My LED Turn ON");
    const struct device *led_driver = shell_device_get_binding("my_led");

    if (led_driver == NULL) {
        shell_error(sh, "my_led device not found.");
        return 1;
    }
    
    return sensor_sample_fetch_chan(led_driver, SENSOR_CHAN_AMBIENT_TEMP);
}

static int my_driver_channel_get_handler(const struct shell *sh, size_t argc, char **argv) {
    shell_info(sh, "My LED Turn OFF");
    const struct device *led_driver = shell_device_get_binding("my_led");

    if (led_driver == NULL) {
        shell_error(sh, "my_led device not found.");
        return 1;
    }
    
    struct sensor_value val;
    return sensor_channel_get(led_driver, SENSOR_CHAN_AMBIENT_TEMP, &val);
}

static int my_driver_info_handler(const struct shell *sh, size_t argc, char **argv) {
    const struct device *led_driver = shell_device_get_binding("my_led");

    if (led_driver == NULL) {
        shell_error(sh, "my_led device not found.");
        return 1;
    }
    
    shell_info(sh, "My LED driver Info. Name=%s, is_ready=%s", 
				"my_led", device_is_ready(led_driver) ? "true" : "false");
    return 0;
}

/* Create subcommands array for "sensor" */
SHELL_STATIC_SUBCMD_SET_CREATE(my_led_subcommands,
    SHELL_CMD_ARG(fetch, NULL, "Turn ON LED.", my_driver_fetch_handler, 1, 0),
    SHELL_CMD_ARG(read, NULL, "Turn OFF LED.", my_driver_channel_get_handler, 1, 0),
    SHELL_CMD_ARG(info, NULL, "Get my_led driver info.", my_driver_info_handler, 1, 0),
    SHELL_SUBCMD_SET_END
);

/* Register toor command "sensor" with subcommands */
SHELL_CMD_REGISTER(sensor, &my_led_subcommands, "My LED commands.", NULL);
