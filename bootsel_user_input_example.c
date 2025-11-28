#include "pico/stdlib.h"
#include "pico/bootrom.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"

// BOOTSEL按键的GPIO连接（需要硬件线）
#define BOOTSEL_USER_GPIO 28

// 按键状态跟踪
static uint32_t boot_press_time = 0;
static bool boot_pressed = false;
static bool boot_long_press_detected = false;

// 初始化BOOTSEL用户功能
void init_bootsel_user_mode() {
    gpio_init(BOOTSEL_USER_GPIO);
    gpio_set_dir(BOOTSEL_USER_GPIO, GPIO_IN);
    gpio_pull_up(BOOTSEL_USER_GPIO);

    printf("BOOTSEL用户模式初始化完成\n");
    printf("短按：模拟鼠标中键\n");
    printf("长按3秒：进入BOOTSEL烧录模式\n");
}

// 处理BOOTSEL按键事件
void handle_bootsel_input() {
    bool current_state = !gpio_get(BOOTSEL_USER_GPIO);  // 读反，因为上拉

    if (current_state && !boot_pressed) {
        // 按键刚按下
        boot_pressed = true;
        boot_press_time = time_us_32();
        boot_long_press_detected = false;
        printf("BOOTSEL按键按下\n");

    } else if (!current_state && boot_pressed) {
        // 按键释放
        boot_pressed = false;
        uint32_t press_duration = time_us_32() - boot_press_time;

        if (boot_long_press_detected) {
            printf("长按事件已处理，忽略释放\n");
        } else if (press_duration > 50000) {  // 50ms以上，有效按键
            printf("BOOTSEL短按：模拟鼠标中键\n");
            // 在这里添加鼠标中键功能
        }

    } else if (current_state && boot_pressed && !boot_long_press_detected) {
        // 按键持续按下，检查长按
        uint32_t press_duration = time_us_32() - boot_press_time;

        if (press_duration > 3000000) {  // 3秒长按
            boot_long_press_detected = true;
            printf("BOOTSEL长按3秒，即将进入BOOTSEL模式...\n");

            // LED闪烁提示
            for(int i = 0; i < 5; i++) {
                gpio_put(PICO_DEFAULT_LED_PIN, 1);
                sleep_ms(200);
                gpio_put(PICO_DEFAULT_LED_PIN, 0);
                sleep_ms(200);
            }

            // 进入BOOTSEL模式
            reset_usb_boot(0, 0);
        }
    }
}

// 多功能按键状态机
typedef enum {
    BOOT_IDLE,
    BOOT_PRESSED,
    BOOT_SHORT_PRESS,
    BOOT_LONG_PRESS,
    BOOT_BOOTSEL_MODE
} boot_state_t;

static boot_state_t current_boot_state = BOOT_IDLE;

void advanced_bootsel_handler() {
    static uint32_t state_timer = 0;
    bool button_pressed = !gpio_get(BOOTSEL_USER_GPIO);
    uint32_t current_time = time_us_32();

    switch(current_boot_state) {
        case BOOT_IDLE:
            if (button_pressed) {
                current_boot_state = BOOT_PRESSED;
                state_timer = current_time;
                printf("进入按键按下状态\n");
            }
            break;

        case BOOT_PRESSED:
            if (!button_pressed) {
                if (current_time - state_timer > 50000) {
                    current_boot_state = BOOT_SHORT_PRESS;
                    printf("检测到短按\n");
                    // 执行短按功能
                } else {
                    current_boot_state = BOOT_IDLE;  // 按下时间太短，忽略
                }
            } else if (current_time - state_timer > 3000000) {
                current_boot_state = BOOT_LONG_PRESS;
                printf("检测到长按\n");
            }
            break;

        case BOOT_SHORT_PRESS:
            // 处理短按功能
            printf("执行短按功能：切换鼠标灵敏度\n");
            current_boot_state = BOOT_IDLE;
            break;

        case BOOT_LONG_PRESS:
            // 执行长按功能
            current_boot_state = BOOT_BOOTSEL_MODE;
            break;

        case BOOT_BOOTSEL_MODE:
            printf("进入BOOTSEL烧录模式\n");
            sleep_ms(100);
            reset_usb_boot(1 << PICO_DEFAULT_LED_PIN, 0);
            break;
    }
}

int main() {
    stdio_init_all();
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

    init_bootsel_user_mode();

    printf("USB鼠标 + BOOTSEL用户输入演示\n");

    while (true) {
        handle_bootsel_input();
        // advanced_bootsel_handler();  // 或使用状态机版本

        // 其他功能...
        sleep_ms(10);
    }

    return 0;
}