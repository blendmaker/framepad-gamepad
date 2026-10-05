#include QMK_KEYBOARD_H
#include "analog.h"

enum layers {
    GAMEPAD,
    MOUSE_KB
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [GAMEPAD] = LAYOUT(
        JS_1, JS_2, JS_3, JS_4,    // D-Pad: Up, Down, Left, Right
        JS_5, JS_6, JS_7, JS_8,    // A, B, X, Y
        JS_9, JS_10, JS_11, JS_12, // L1, L2, L3, R1
        JS_13, JS_14, JS_15, JS_16 // R2, R3, Start, Select
    ),
    [MOUSE_KB] = LAYOUT(
        KC_UP, KC_DOWN, KC_LEFT, KC_RIGHT, // D-Pad
        KC_ENT, KC_ESC, KC_SPC, KC_TRNS,   // A, B, X, Y
        MS_WHLD, MS_BTN1, KC_TRNS, MS_WHLU, // L1, L2, L3, R1
        MS_BTN2, KC_TRNS, KC_ENT, KC_ESC    // R2, R3, Start, Select
    )
};

joystick_config_t joystick_axes[JOYSTICK_AXIS_COUNT] = {
    [0] = JOYSTICK_AXIS_IN(GP26, 0, 2048, 4095), // Left stick X
    [1] = JOYSTICK_AXIS_IN(GP27, 0, 2048, 4095), // Left stick Y
    [2] = JOYSTICK_AXIS_IN(GP28, 0, 2048, 4095), // Right stick X
    [3] = JOYSTICK_AXIS_IN(GP29, 0, 2048, 4095)  // Right stick Y
};

static uint32_t l3r3_hold_start = 0;
static bool     l3r3_holding    = false;
static bool     l3r3_toggled    = false;

void matrix_scan_user(void) {
    if (matrix_is_on(0, 10) && matrix_is_on(0, 13)) { // L3 + R3
        if (!l3r3_holding) {
            l3r3_hold_start = timer_read();
            l3r3_holding    = true;
            l3r3_toggled    = false;
        } else if (!l3r3_toggled && timer_elapsed(l3r3_hold_start) >= 2000) {
            layer_move(get_highest_layer(layer_state) == GAMEPAD ? MOUSE_KB : GAMEPAD);
            l3r3_toggled = true;
        }
    } else {
        l3r3_holding = false;
        l3r3_toggled = false;
    }
}

report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
    if (get_highest_layer(layer_state) == MOUSE_KB) {
        int16_t rx = analogReadPin(GP28) - 2048; // Right stick X -> cursor
        int16_t ry = analogReadPin(GP29) - 2048; // Right stick Y -> cursor
        int16_t lx = analogReadPin(GP26) - 2048; // Left stick X -> hscroll
        int16_t ly = analogReadPin(GP27) - 2048; // Left stick Y -> vscroll

        if (rx > -64 && rx < 64) rx = 0;
        if (ry > -64 && ry < 64) ry = 0;
        if (lx > -128 && lx < 128) lx = 0;
        if (ly > -128 && ly < 128) ly = 0;

        mouse_report.x = CONSTRAIN_HID(rx / 64);
        mouse_report.y = CONSTRAIN_HID(ry / 64);
        mouse_report.h = CONSTRAIN_HID(lx / 256);
        mouse_report.v = CONSTRAIN_HID(ly / 256);
    }
    return mouse_report;
}
