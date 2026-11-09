#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/shell/shell.h>

#include <my_led.h>

#define LED_RED     DT_ALIAS(driver_led)
#define LED_BLUE    DT_ALIAS(driver_led_extra)
#define MY_LED      DT_NODELABEL(my_led)

static const struct device* led_driver      = DEVICE_DT_GET(MY_LED);
static const struct gpio_dt_spec led_red    = GPIO_DT_SPEC_GET(LED_RED, gpios);
static const struct gpio_dt_spec led_blue   = GPIO_DT_SPEC_GET(LED_BLUE, gpios);

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

int main(void)
{
    bool led_state = true;

    if (!gpio_is_ready_dt(&led_red)) return 0;
	LOG_INF("LED RED ready");
	
    if (gpio_pin_configure_dt(&led_red, GPIO_OUTPUT_INACTIVE) < 0) return 0;
	LOG_INF("LED RED configured");

    if (!gpio_is_ready_dt(&led_blue)) return 0;
	LOG_INF("LED BLUE ready");
	
    if (gpio_pin_configure_dt(&led_blue, GPIO_OUTPUT_INACTIVE) < 0) return 0;
	LOG_INF("LED BLUE configured");

    if (my_led_set_gpio(led_driver, &led_red) != 0) return 0;
    LOG_INF("LED RED set to LED driver");

    return 0;
}
