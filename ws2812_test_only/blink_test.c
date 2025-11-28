/**
 * 最简单的WS2812测试 - 只点亮一个LED
 * GP16连接WS2812的数据引脚
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "ws2812.pio.h"

#define WS2812_PIN 16
#define NUM_PIXELS 1

int main() {
    stdio_init_all();
    printf("WS2812 Blink Test - GP%d\n", WS2812_PIN);

    PIO pio;
    uint sm;
    uint offset;

    // 初始化WS2812
    pio_claim_free_sm_and_add_program_for_gpio_range(
        &ws2812_program, &pio, &sm, &offset, WS2812_PIN, 1, true);
    ws2812_program_init(pio, sm, offset, WS2812_PIN, 800000, false);

    uint32_t pixel_grb = 0x0f0000; // 低亮度绿色 (GRB格式)

    while (true) {
        // 发送像素数据
        pio_sm_put_blocking(pio, sm, pixel_grb << 8u);
        sleep_ms(500);

        // 发送关闭
        pio_sm_put_blocking(pio, sm, 0);
        sleep_ms(500);
    }
}