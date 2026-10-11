#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(board_early_init_hook, LOG_LEVEL_INF);

void board_early_init_hook(void) {
    LOG_INF("Board initialized");
}