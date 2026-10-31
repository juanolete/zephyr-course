#include <zephyr/drivers/sensor.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

static const struct device* driver = DEVICE_DT_GET(DT_NODELABEL(my_led));
struct sensor_value val;

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

int main(void)
{
    bool led_state = true;

    while (1) {
        if (led_state) {
            // Turn Off LED
            if (sensor_channel_get(driver, SENSOR_CHAN_AMBIENT_TEMP, &val)) return 0;
        } else {
            // Turn On LED
            if (sensor_sample_fetch_chan(driver, SENSOR_CHAN_AMBIENT_TEMP)) return 0;
        }
        led_state = !led_state;
        
        LOG_INF("LED state: %s", led_state ? "ON" : "OFF");
        k_msleep(CONFIG_APP_HEARTBEAT_PERIOD_MS);
    }
    return 0;
}
