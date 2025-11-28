cmake_minimum_required(VERSION 3.13)
set(CMAKE_C_STANDARD 11)

set(PICO_SDK_PATH "/Users/gx1000/code/pico-sdk")
include(pico_sdk_import.cmake)

project(ws2812_blink_test C CXX ASM)
pico_sdk_init()

add_executable(ws2812_blink_test blink_test.c)

target_link_libraries(ws2812_blink_test
        pico_stdlib
        hardware_pio)

pico_generate_pio_header(ws2812_blink_test ${CMAKE_CURRENT_SOURCE_DIR}/../ws2812.pio)

pico_add_extra_outputs(ws2812_blink_test)