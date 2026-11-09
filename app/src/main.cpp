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

int main(void)
{
    bool led_state = true;

    if (!gpio_is_ready_dt(&led_gpio)) return 0;
	LOG_INF("GPIO ready");
	
    if (gpio_pin_configure_dt(&led_gpio, GPIO_OUTPUT_ACTIVE) < 0) return 0;
	LOG_INF("GPIO configured");

    if (my_led_set_gpio(led_driver, &led_gpio) != 0) return 0;
    LOG_INF("GPIO set to LED driver");

    return 0;
}
