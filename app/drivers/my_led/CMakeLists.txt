if (CONFIG_MY_LED)

target_sources(app PRIVATE src/my_led.c)
target_include_directories(app PUBLIC include)

endif()