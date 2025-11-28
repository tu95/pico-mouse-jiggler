#include "pico/stdlib.h"
#include "pico/bootrom.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"

// 使用BOOTSEL按钮切换模式（需要硬件连接）
// 如果将BOOTSEL按键连接到GPIO，可以在代码中检测
#define BOOTSEL_DETECT_PIN 28  // 假设BOOTSEL按键连接到GPIO28

// 简单的延时函数
void delay_ms(uint32_t ms) {
    sleep_ms(ms);
}

int main() {
    stdio_init_all();

    // 初始化ADC和GPIO
    adc_init();
    adc_gpio_init(26); // X轴
    adc_gpio_init(27); // Y轴

    gpio_init(5);  // 左键
    gpio_init(6);  // 右键
    gpio_init(7);  // 中键
    gpio_set_dir(5, GPIO_IN);
    gpio_set_dir(6, GPIO_IN);
    gpio_set_dir(7, GPIO_IN);
    gpio_pull_up(5);
    gpio_pull_up(6);
    gpio_pull_up(7);

    // 可选：连接BOOTSEL到GPIO进行检测
    #ifdef BOOTSEL_DETECT_PIN
    gpio_init(BOOTSEL_DETECT_PIN);
    gpio_set_dir(BOOTSEL_DETECT_PIN, GPIO_IN);
    gpio_pull_up(BOOTSEL_DETECT_PIN);
    #endif

    uint32_t boot_hold_count = 0;

    printf("USB Mouse 启动完成！\n");
    printf("开发模式：\n");
    printf("- 长按用户按键3秒进入BOOTSEL模式\n");

    while (true) {
        #ifdef BOOTSEL_DETECT_PIN
        // 检测BOOTSEL按键是否被按住（仅用于演示）
        if (!gpio_get(BOOTSEL_DETECT_PIN)) {
            boot_hold_count++;
            printf("检测到BOOTSEL按键: %d/300 (约3秒)\n", boot_hold_count / 100);

            if (boot_hold_count > 300) {  // 约3秒
                printf("进入BOOTSEL模式...\n");
                delay_ms(100);
                reset_usb_boot(0, 0);  // 进入USB存储模式
            }
        } else {
            boot_hold_count = 0;
        }
        #endif

        // 正常的鼠标功能代码...
        // ... (原有的USB鼠标代码)

        delay_ms(10);
    }

    return 0;
}