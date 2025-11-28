#pragma once

#ifdef __cplusplus
extern "C" {
#endif

// Board configuration
#define CFG_TUSB_MCU                OPT_MCU_RP2040

// USB Device configuration
#define CFG_TUD_ENABLED             1

// USB Device endpoint count
#define CFG_TUD_ENDPOINT0_SIZE       64

// Disable CDC (Serial) - we only need HID
#define CFG_TUD_CDC                 0
#define CFG_TUD_MSC                 0
#define CFG_TUD_HID                 1

// HID configuration
#define CFG_TUD_HID_EPIN_BUFSIZE    64
#define CFG_TUD_HID_EPOUT_BUFSIZE   64

// HID mouse
#define CFG_TUD_HID_MOUSE           1

// USB power and configuration
#define CFG_TUD_MAX_EP_BUFSIZE      64
#define CFG_TUD_MAX_ITF             4
#define CFG_TUD_MAX_STRING_LEN      32

// VID/PID Configuration
#define CFG_TUD_VID                 0xCAFE  // Vendor ID
#define CFG_TUD_PID                 0x0001  // Product ID

// Self-powered configuration (optional)
#define CFG_TUD_SELF_POWERED         0

// USB Remote wakeup
#define CFG_TUD_REMOTE_WAKEUP        0

// Control transfer buffer size
#define CFG_TUD_CONTROL_REQUEST_BUFSIZE 256

// HID report descriptors
#define CFG_TUD_HID_MOUSE_REPORT_DESC_LEN 66

// RP2040 specific configuration
#define BOARD_DEVICE_RHPORT_NUM      0
#define BOARD_DEVICE_RHPORT_SPEED    OPT_MODE_FULL_SPEED

// RHport mode configuration
#define CFG_TUSB_RHPORT0_MODE       (OPT_MODE_DEVICE | OPT_MODE_FULL_SPEED)

#ifdef __cplusplus
}
#endif