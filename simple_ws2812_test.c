/**
 * 简单的WS2812测试程序 - 只有一个LED，GP16
 * 这个程序只做一件事：点亮WS2812
 */

#include <stdio.h>
#include <stdlib.h>

#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "ws2812.pio.h"

// 使用GP16，简化配置
#define WS2812_PIN 16
#define IS_RGBW false
#define NUM_PIXELS 1

static inline void put_pixel(PIO pio, uint sm, uint32_t pixel_grb) {
    pio_sm_put_blocking(pio, sm, pixel_grb << 8u);
}

static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b) {
    return
            ((uint32_t) (r) << 8) |
            ((uint32_t) (g) << 16) |
            (uint32_t) (b);
}

int main() {
    stdio_init_all();
    printf("Simple WS2812 Test - Single LED on GP%d\n", WS2812_PIN);

    // 等待串口初始化
    sleep_ms(2000);

    PIO pio;
    uint sm;
    uint offset;

    // 查找免费的PIO和状态机，加载程序
    bool success = pio_claim_free_sm_and_add_program_for_gpio_range(
        &ws2812_program, &pio, &sm, &offset, WS2812_PIN, 1, true);

    if (!success) {
        printf("ERROR: Failed to claim PIO or state machine!\n");
        while (1) {
            tight_loop_contents();
        }
    }

    // 初始化WS2812程序 - 800kHz频率
    ws2812_program_init(pio, sm, offset, WS2812_PIN, 800000, IS_RGBW);
    printf("WS2812 initialized successfully\n");

    // 测试序列
    printf("Starting LED test sequence...\n");

    while (1) {
        // 红色
        printf("RED\n");
        put_pixel(pio, sm, urgb_u32(255, 0, 0));  // RGB Red
        sleep_ms(1000);

        // 绿色
        printf("GREEN\n");
        put_pixel(pio, sm, urgb_u32(0, 255, 0));  // RGB Green
        sleep_ms(1000);

        // 蓝色
        printf("BLUE\n");
        put_pixel(pio, sm, urgb_u32(0, 0, 255));  // RGB Blue
        sleep_ms(1000);

        // 白色
        printf("WHITE\n");
        put_pixel(pio, sm, urgb_u32(255, 255, 255));  // RGB White
        sleep_ms(1000);

        // 关闭
        printf("OFF\n");
        put_pixel(pio, sm, 0);
        sleep_ms(1000);

        printf("Cycle completed. Starting again...\n\n");
    }

    // 通常不会到达这里，但为了完整性
    pio_remove_program_and_unclaim_sm(&ws2812_program, pio, sm, offset);
    return 0;
}