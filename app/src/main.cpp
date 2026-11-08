#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/shell/shell.h>

#include <my_led.h>

/* "driver_led" alias defined in app.overlay file. */
#define LED_HEARTBEAT   DT_ALIAS(driver_led)

static const struct device* led_driver = DEVICE_DT_GET(DT_NODELABEL(my_led));
static const struct gpio_dt_spec led_gpio = GPIO_DT_SPEC_GET(LED_HEARTBEAT, gpios);

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

static int cmd_version(const struct shell * sh, size_t argc, char **argv) {
    shell_print(sh, "App version: 1.0.0");
    return 0;
}

/* Register version command */
SHELL_CMD_REGISTER(version, NULL, "Show app version.", cmd_version);

static int cmd_demo_salute(const struct shell *sh, size_t argc, char **argv) {
    shell_print(sh, "Good Afternoon");
    return 0;
}

static int cmd_demo_parse(const struct shell *sh, size_t argc, char **argv) {
    for (int i = 0; i < argc; i++) {
        shell_print(sh, "   argv[%d] = %s", i, argv[i]);
    }
    return 0;
}

/* Create subcommands array for "demo" */
SHELL_STATIC_SUBCMD_SET_CREATE(sub_demo,
    SHELL_CMD(salute, NULL, "Salute command.", cmd_demo_salute),
    SHELL_CMD(parse, NULL, "Print params command.", cmd_demo_parse),
    SHELL_SUBCMD_SET_END
);

/* Register toor command "demo" with subcommands */
SHELL_CMD_REGISTER(demo, &sub_demo, "Demo commands.", NULL);

int main(void)
{
    bool led_state = true;

    if (!gpio_is_ready_dt(&led_gpio)) return 0;
	LOG_INF("GPIO ready");
	
    if (gpio_pin_configure_dt(&led_gpio, GPIO_OUTPUT_ACTIVE) < 0) return 0;
	LOG_INF("GPIO configured");

    if (my_led_set_gpio(led_driver, &led_gpio) != 0) return 0;
    LOG_INF("GPIO set to LED driver");

    while (1) {
        if (led_state) {
            // Turn Off LED
            struct sensor_value val;
            if (sensor_channel_get(led_driver, SENSOR_CHAN_AMBIENT_TEMP, &val)) return 0;
        } else {
            // Turn On LED
            if (sensor_sample_fetch_chan(led_driver, SENSOR_CHAN_AMBIENT_TEMP)) return 0;
        }
        led_state = !led_state;
        
        k_msleep(CONFIG_APP_HEARTBEAT_PERIOD_MS);
    }
    return 0;
}
