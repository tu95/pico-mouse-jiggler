# USB Mouse for Raspberry Pi Pico

This project turns a Raspberry Pi Pico into a USB mouse device that can be connected to a computer and used as a regular mouse.

## Features

- **3 Mouse Buttons**: Left, right, and middle mouse buttons
- **Analog Movement**: X and Y axis movement using analog inputs (potentiometers or joystick)
- **USB HID Interface**: Full USB Human Interface Device (HID) implementation
- **Low Latency**: 10ms report interval for responsive movement
- **LED Indicator**: Built-in LED shows connection status

## Hardware Requirements

### Required Components
- Raspberry Pi Pico
- 3 x Push buttons (for mouse buttons)
- 2 x 10kΩ potentiometers or an analog joystick
- Breadboard and jumper wires

### Pin Connections

#### Mouse Buttons (Active Low)
- Left Button: GPIO 5
- Right Button: GPIO 6
- Middle Button: GPIO 7

#### Movement Inputs
- X Axis: GPIO 26 (ADC0)
- Y Axis: GPIO 27 (ADC1)

#### LED Indicator
- Status LED: GPIO 25 (built-in LED on Pico)

### Circuit Diagram

```
Pico               Buttons                Potentiometers/Joystick
GPIO 5 --------[Button]------- GND        GPIO 26 --------[Pot]------- GND
GPIO 6 --------[Button]------- GND        GPIO 27 --------[Pot]------- GND
GPIO 7 --------[Button]------- GND                           |
           (All buttons    |                              3.3V
            need 10kΩ       |
            pull-up resistors)

GPIO 25 -------------------- LED -------------------- GND
```

## Software Requirements

- Pico SDK 1.4.0 or later
- ARM GCC toolchain
- CMake 3.13 or later

## Building and Installation

1. **Set up the build environment:**
   ```bash
   cd usb_mouse
   mkdir build
   cd build
   ```

2. **Configure the project:**
   ```bash
   cmake -DCMAKE_BUILD_TYPE=Debug ..
   ```

3. **Build the project:**
   ```bash
   make -j$(nproc)
   ```

4. **Upload to Pico:**
   - Press and hold the BOOTSEL button on your Pico
   - Connect the Pico to your computer via USB
   - Drag the `usb_mouse.uf2` file to the Pico drive

## Usage

1. **Connect the hardware** as shown in the circuit diagram
2. **Upload the firmware** to your Pico
3. **Connect the Pico** to your target computer via USB
4. The device will appear as a standard USB mouse
5. Use the buttons and analog inputs to control the mouse

## Configuration Options

### Sensitivity Adjustment
You can adjust the mouse sensitivity by modifying the division factor in `read_movement()`:
```c
// Current setting: divide by 16
mouse_x = (int8_t)((center - x_raw) / 16);

// For higher sensitivity: divide by 8
mouse_x = (int8_t)((center - x_raw) / 8);

// For lower sensitivity: divide by 32
mouse_x = (int8_t)((center - x_raw) / 32);
```

### Deadzone Adjustment
The deadzone prevents accidental movement when the joystick is centered:
```c
// Current setting: 100
const uint16_t deadzone = 100;

// For tighter control: increase to 200
const uint16_t deadzone = 200;

// For more sensitive control: decrease to 50
const uint16_t deadzone = 50;
```

## Customization

### Different Input Types
- **Digital Movement**: Replace the ADC reading with digital inputs for discrete movement
- **Additional Buttons**: Add more GPIO pins for extra mouse buttons
- **Scroll Wheel**: Add another pot or encoder for scroll functionality

### Performance Tuning
- **Report Rate**: Change `MOUSE_REPORT_INTERVAL_MS` for different polling rates
- **Movement Range**: Modify the conversion logic for different movement ranges

## Troubleshooting

### Device Not Recognized
- Check USB connections
- Ensure the BOOTSEL process completed successfully
- Try a different USB cable

### Movement Issues
- Check potentiometer connections
- Verify 3.3V power to analog inputs
- Adjust sensitivity and deadzone settings

### Button Problems
- Ensure proper pull-up resistors
- Check button wiring (active low configuration)
- Verify button functionality with a multimeter

## Technical Details

### Implementation
- Uses TinyUSB stack for USB HID implementation
- 12-bit ADC for analog input (0-4095 range)
- 8-bit signed values for mouse movement (-127 to +127)
- Hardware debouncing for button inputs

### USB HID Report Format
The device sends standard HID mouse reports with:
- 1 byte: Button bitmap
- 1 byte: X movement
- 1 byte: Y movement
- 1 byte: Wheel movement
- 1 byte: Pan movement

## License

This project follows the same license as the Pico SDK (BSD-3-Clause).

## 工作记录

- 2025-11-27：将 `usb_mouse.c` 中手写的 WS2812 PIO 配置替换为官方 `ws2812_program`，改用 `pio_claim_free_sm_and_add_program_for_gpio_range` 和 `ws2812_program_init`，LED 逻辑统一为 RGB→GRB 转换后调用 `put_pixel`，确保 GP16 单颗灯能与参考示例一致工作。
- 2025-11-27：在 `CMakeLists.txt` 中加入 `pico_generate_pio_header` 生成 `ws2812.pio.h`，构建流程改为 `cmake -S . -B build` 与 `cmake --build build`，验证通过。*** End Patch*** 