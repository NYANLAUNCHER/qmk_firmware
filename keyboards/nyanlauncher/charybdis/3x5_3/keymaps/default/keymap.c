/**
 * Copyright 2021 Charly Delay <charly@codesink.dev> (@0xcharly)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "print.h"
#include "charybdis.h"
#include "keymap.h"

// Precedence goes from 31 -> 0
// QMK finds the first non-transparent key starting from current layer going down
enum charybdis_keymap_layers {
    _DEFAULT,// QWERTY
    _DVORAK,
    _GMG,// ESDF -> WASD
    _GMG1,// Moar Letters
    _GMG2,// Numbers and Symbols
    _GMG3,// function keys
    _LKEYPAD,// Numpad on Left
    _RKEYPAD,// Numpad on Right
    _SYMBOLS,// Common Symbols
    _SYMBOLS1,// Extra Symbols
    _NAV,// Navigation, Media, & Mouse keys
    _MOUSE,// Auto-Mouse Layer
    _RAISE,// Modifers, Numbers, and other things
    _FN,// All the function keys
    _QKC,// Quantum Keycodes, Debugging, and Kb Macros
    _LAYERS// Access all layers (must be the last layer)
};
// All custom keycodes must be >= SAFE_RANGE
enum custom_keycodes {
    SCROLL_TOGG=SAFE_RANGE,// toggles tb_scroll
    SCWC_ENABLE,
    SCWC_DISABLE,
    SCWC_TOGG,
    KC_LLCK,
    TB_SCROLL,// activate tb_scroll while pressed
    STICKY_MODS,// tapping enables, holding clears sticky mods
    LAYER_TO,// TG() keys act like TO() while pressed
    // gaming LT() aliases
    GMG1_ESC,
    GMG2_SPC,
    GMG3_LCTL
};

//uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record)
//    return TAPPING_TERM;// default value
//}

static bool tb_scroll = false;// enables and disables scrolling for the trackbal
void keyboard_post_init_user() {
    //eeconfig_init();
    debug_enable=true;// enable debugging by default
    debug_mouse=true;
    //debug_keyboard=true;
    //debug_matrix=true
}
//void pointing_device_init_user(void) {
    //set_auto_mouse_layer(_MOUSE);
    //set_auto_mouse_enable(true);
//}
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    static uint16_t prev_time;
    switch (keycode) {
    // Custom Keycodes:
    case TB_SCROLL:
        if (record->event.pressed) { tb_scroll=true; } else { tb_scroll=false; }
        keycode=KC_NO; break;
    case SCROLL_TOGG:
        if (record->event.pressed) tb_scroll=!tb_scroll;
        keycode=KC_NO; break;
    // Gaming layer swap keys
    case GMG1_ESC:
        keycode=LT(_GMG1, KC_ESC);
        break;
    case GMG2_SPC:
        keycode=LT(_GMG2, KC_ESC);
        break;
    case GMG3_LCTL:
        keycode=LT(_GMG3, KC_LCTL);
        break;
    // Normal Codes
    /*case KC_LLCK:
        if (record->event.pressed) {
            register_code(TG(get_highest_layer(layer_state)));
        } else {
            unregister_code(TG(get_highest_layer(layer_state)));
        }
        break;*/
    }
    dprintf("key: 0x%x; pressed: %s; time:%s%dms (%d,%d)\n",
         keycode,
         record->event.pressed ? "true" : "false",
         record->event.pressed ? "  " : " +",
         (timer_read()-prev_time),
         record->event.key.col, record->event.key.row
    );
    prev_time=timer_read();//used on next invocation
    return true;
}
// Trackball Customization:
report_mouse_t pointing_device_task_combined_user(report_mouse_t left_report, report_mouse_t right_report) {
    left_report.x=-left_report.x;
    left_report.h=-left_report.h;
    right_report.y=-right_report.y;
    right_report.v=-right_report.v;
    return pointing_device_combine_reports(left_report, right_report);
}
report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
    static float scroll_accumulated_h = 0;
    static float scroll_accumulated_v = 0;
    if (tb_scroll) {
        // Calculate and accumulate scroll values based on mouse movement and divisors
        scroll_accumulated_h += (float)mouse_report.x / SCROLL_DIVISOR_H;
        scroll_accumulated_v += (float)(-mouse_report.y) / SCROLL_DIVISOR_V;
        // Assign integer parts of accumulated scroll values to the mouse report
        mouse_report.h = (int8_t)scroll_accumulated_h;
        mouse_report.v = (int8_t)scroll_accumulated_v;
        // Update accumulated scroll values by subtracting the integer parts
        scroll_accumulated_h -= mouse_report.h;
        scroll_accumulated_v -= mouse_report.h;
        // Clear the X and Y values of the mouse report
        mouse_report.x = 0;
        mouse_report.y = 0;
    }
    return mouse_report;
}


// clang-format off
#define _3D _DEFAULT
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_DEFAULT] = LAYOUT(
  //   ╭────────────────────────────────────────────╮ ╭──────────────────────────────────────────────╮
          KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,     KC_Y,   KC_U,    KC_I,    KC_O,    KC_P,
  //   ├────────────────────────────────────────────┤ ├──────────────────────────────────────────────┤
          KC_A,    KC_S,    KC_D,    KC_F,    KC_G,     KC_H,   KC_J,    KC_K,    KC_L,    KC_SCLN,
  //   ├────────────────────────────────────────────┤ ├──────────────────────────────────────────────┤
          KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,     KC_N,   KC_M,   KC_COMM,  KC_DOT,  KC_SLSH,
  //   ╰────────────────────────────────────────────┤ ├──────────────────────────────────────────────╯
             KC_LGUI, CTL_SPC, LT(_SYMBOLS,KC_ENT),     TT(_RAISE), SFT_BSPC, KC_ESC
  //       ╰────────────────────────────────────────╯ ╰──────────────────────────────╯
  ),

  [_RAISE] = LAYOUT(
  // ╭──────────────────────────────────────────────────╮ ╭───────────────────────────────────────────────────╮
           KC_1,    KC_2,     KC_3,     KC_4,    KC_5,      KC_6,     KC_7,     KC_8,     KC_9,    KC_0,
  // ├──────────────────────────────────────────────────┤ ├───────────────────────────────────────────────────┤
        KC_LGUI, KC_LALT,  KC_LCTL,  KC_LSFT, TG(_GMG),     TG(_NAV), KC_RSFT, KC_RCTL, KC_RALT,  KC_RGUI,
  // ├──────────────────────────────────────────────────┤ ├───────────────────────────────────────────────────┤
        TG(_3D), SCWC_TOGG, KO_TOGG, QK_BOOT, TG(_QKC),     KC_LEFT,  KC_DOWN, KC_UP,  KC_RIGHT, STICKY_MODS,
  // ╰──────────────────────────────────────────────────┤ ├───────────────────────────────────────────────────╯
                               KC_DEL, KC_TAB, MO(_FN),     TG(_LAYERS), TG(_RKEYPAD), TO(_DEFAULT)
  //                         ╰──────────────────────────╯ ╰─────────────────────────────────────────╯
  ),

  // Symbols: {{{
  [_SYMBOLS] = LAYOUT(
  //  ╭──────────────────────────────────────────────╮ ╭─────────────────────────────────────────────╮
        KC_TILD,  KC_RBRC, KC_RCBR, KC_RPRN, KC_PERC,     KC_CIRC, KC_PIPE, KC_DQUO, XXXXXXX, KC_BSLS,
  //  ├──────────────────────────────────────────────┤ ├─────────────────────────────────────────────┤
        KC_EXLM, KC_LBRC, KC_LCBR, KC_LPRN, KC_AMPR,     KC_ASTR, KC_EQL,  KC_PLUS, KC_MINS, KC_UNDS,
  //  ├──────────────────────────────────────────────┤ ├─────────────────────────────────────────────┤
        KC_GRV,   KC_AT, KC_HASH,  KC_DLR, XXXXXXX,     XXXXXXX, KC_QUOT, KC_LT,   KC_GT,   KC_SLSH,
  //  ╰──────────────────────────────────────────────┤ ├─────────────────────────────────────────────╯
                          _______, _______, XXXXXXX,     MO(_SYMBOLS1), _______, TO(_DEFAULT)
  //                    ╰────────────────────────────╯ ╰──────────────────────────────────────╯
  ),
  [_SYMBOLS1] = LAYOUT(
  //  ╭──────────────────────────────────────────────╮ ╭─────────────────────────────────────────────╮
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //  ├──────────────────────────────────────────────┤ ├─────────────────────────────────────────────┤
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //  ├──────────────────────────────────────────────┤ ├─────────────────────────────────────────────┤
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //  ╰──────────────────────────────────────────────┤ ├─────────────────────────────────────────────╯
                          _______, _______, _______,     XXXXXXX, _______, TO(_DEFAULT)
  //                    ╰────────────────────────────╯ ╰────────────────────────────────╯
  ),
  // }}}

  // Gaming: {{{
  [_GMG] = LAYOUT(
  //     ╭──────────────────────────────────────────╮ ╭──────────────────────────────────────────────╮
            KC_LALT,  KC_Q,   KC_W,   KC_E,   KC_R,     KC_Y,   KC_U,    KC_I,    KC_O,    KC_P,
  //     ├──────────────────────────────────────────┤ ├──────────────────────────────────────────────┤
            KC_LSFT,  KC_A,   KC_S,   KC_D,   KC_F,     KC_H,   KC_J,    KC_K,    KC_L,    KC_SCLN,
  //     ├──────────────────────────────────────────┤ ├──────────────────────────────────────────────┤
            KC_TAB,   KC_Z,   KC_X,   KC_C,   KC_V,     KC_N,   KC_M,   KC_COMM,  KC_DOT,  KC_SLSH,
  //     ╰──────────────────────────────────────────┤ ├──────────────────────────────────────────────╯
                     //GMG1_ESC, GMG2_SPC, GMG3_LCTL,     KC_ESC, MT(MOD_RSFT,KC_ENT), TO(_DEFAULT)
LT(_GMG1,KC_ESC),LT(_GMG2,KC_SPC),LT(_GMG3,KC_LCTL),     KC_ESC, MT(MOD_RSFT,KC_ENT), TO(_DEFAULT)
  //               ╰────────────────────────────────╯ ╰───────────────────────────────────────────╯
  ),
  [_GMG1] = LAYOUT(
  //  ╭────────────────────────────────────────────╮ ╭─────────────────────────────────────────────╮
           KC_P,     KC_O,   KC_I,   KC_U,   KC_Y,     KC_PPLS,   KC_P7,   KC_P8,  KC_P9, KC_PMNS,
  //  ├────────────────────────────────────────────┤ ├─────────────────────────────────────────────┤
           KC_G,     KC_L,   KC_K,   KC_J,   KC_H,     KC_PAST,   KC_P4,   KC_P5,  KC_P6, KC_PSLS,
  //  ├────────────────────────────────────────────┤ ├─────────────────────────────────────────────┤
          KC_SCLN,   KC_M,   KC_N,   KC_B,   KC_T,     KC_PDOT,   KC_P1,   KC_P2,  KC_P3, KC_PENT,
  //  ╰────────────────────────────────────────────┤ ├─────────────────────────────────────────────╯
                       TG(_GMG), XXXXXXX, XXXXXXX,     KC_NUM, KC_BSPC, TO(_DEFAULT)
  //                 ╰─────────────────────────────╯ ╰───────────────────────────────╯
  ),
  [_GMG2] = LAYOUT(
  //  ╭───────────────────────────────────────────────╮ ╭──────────────────────────────────────────────╮
          KC_6,     KC_7,    KC_8,    KC_9,    KC_0,      _______, _______, _______, _______, _______,
  //  ├───────────────────────────────────────────────┤ ├──────────────────────────────────────────────┤
          KC_1,     KC_2,    KC_3,    KC_4,    KC_5,      _______, _______, _______, _______, _______,
  //  ├───────────────────────────────────────────────┤ ├──────────────────────────────────────────────┤
         KC_LBRC, KC_RBRC, KC_MINS, KC_PLUS, KC_COMM,     _______, _______, _______, _______, _______,
  //  ╰───────────────────────────────────────────────┤ ├──────────────────────────────────────────────╯
                          TG(_GMG), XXXXXXX, XXXXXXX,     _______, _______, _______
  //                    ╰─────────────────────────────╯ ╰───────────────────────────╯
  ),
  [_GMG3] = LAYOUT(
  //  ╭─────────────────────────────────────────────╮ ╭──────────────────────────────────────────────╮
         KC_F1,   KC_F2,   KC_F3,  KC_F4,   KC_F5,      _______, _______, _______, _______, _______,
  //  ├─────────────────────────────────────────────┤ ├──────────────────────────────────────────────┤
         KC_F6,   KC_F7,   KC_F8,  KC_F9,   KC_F10,     _______, _______, _______, _______, _______,
  //  ├─────────────────────────────────────────────┤ ├──────────────────────────────────────────────┤
         KC_ENT,  KC_SLSH, KC_GRV, KC_F11,  KC_F12,     _______, _______, _______, _______, _______,
  //  ╰─────────────────────────────────────────────┤ ├──────────────────────────────────────────────╯
                        TG(_GMG), XXXXXXX, XXXXXXX,     _______, _______, _______
  //                  ╰─────────────────────────────╯ ╰───────────────────────────╯
  ),
  //}}}

  /* 3D: {{{
  [_3D] = LAYOUT(
  //        ╭──────────────────────────────────────────╮ ╭──────────────────────────────────────────────╮
               KC_LALT,  KC_Q,   KC_W,   KC_E,   KC_R,     KC_Y,   KC_U,    KC_I,    KC_O,    KC_P,
  //        ├──────────────────────────────────────────┤ ├──────────────────────────────────────────────┤
               KC_LSFT,  KC_A,   KC_S,   KC_D,   KC_F,     KC_H,   KC_J,    KC_K,    KC_L,    KC_SCLN,
  //        ├──────────────────────────────────────────┤ ├──────────────────────────────────────────────┤
               KC_TAB,   KC_Z,   KC_X,   KC_C,   KC_V,     KC_N,   KC_M,   KC_COMM,  KC_DOT,  KC_SLSH,
  //        ╰──────────────────────────────────────────┤ ├──────────────────────────────────────────────╯
      LT(_3D1,KC_ESC),LT(_3D2,KC_SPC),LT(_3D3,KC_LCTL),    KC_ESC, MT(MOD_RSFT,KC_ENT), TO(_DEFAULT)
  //                  ╰────────────────────────────────╯ ╰───────────────────────────────────────────╯
  ),
  [_3D1] = LAYOUT(
  //  ╭────────────────────────────────────────────╮ ╭─────────────────────────────────────────────╮
           KC_P,     KC_O,   KC_I,   KC_U,   KC_Y,     KC_PPLS,   KC_P7,   KC_P8,  KC_P9, KC_PMNS,
  //  ├────────────────────────────────────────────┤ ├─────────────────────────────────────────────┤
           KC_G,     KC_L,   KC_K,   KC_J,   KC_H,     KC_PAST,   KC_P4,   KC_P5,  KC_P6, KC_PSLS,
  //  ├────────────────────────────────────────────┤ ├─────────────────────────────────────────────┤
          KC_SCLN,   KC_M,   KC_N,   KC_B,   KC_T,     KC_PDOT,   KC_P1,   KC_P2,  KC_P3, KC_PENT,
  //  ╰────────────────────────────────────────────┤ ├─────────────────────────────────────────────╯
                       TG(_GMG), XXXXXXX, XXXXXXX,     KC_NUM, KC_BSPC, TO(_DEFAULT)
  //                 ╰─────────────────────────────╯ ╰───────────────────────────────╯
  ),
  [_3D2] = LAYOUT(
  //  ╭───────────────────────────────────────────────╮ ╭──────────────────────────────────────────────╮
          KC_6,     KC_7,    KC_8,    KC_9,    KC_0,      _______, _______, _______, _______, _______,
  //  ├───────────────────────────────────────────────┤ ├──────────────────────────────────────────────┤
          KC_1,     KC_2,    KC_3,    KC_4,    KC_5,      _______, _______, _______, _______, _______,
  //  ├───────────────────────────────────────────────┤ ├──────────────────────────────────────────────┤
         KC_LBRC, KC_RBRC, KC_MINS, KC_PLUS, KC_COMM,     _______, _______, _______, _______, _______,
  //  ╰───────────────────────────────────────────────┤ ├──────────────────────────────────────────────╯
                          TG(_GMG), XXXXXXX, XXXXXXX,     _______, _______, _______
  //                    ╰─────────────────────────────╯ ╰───────────────────────────╯
  ),
  [_3D3] = LAYOUT(
  //  ╭─────────────────────────────────────────────╮ ╭──────────────────────────────────────────────╮
         KC_F1,   KC_F2,   KC_F3,  KC_F4,   KC_F5,      _______, _______, _______, _______, _______,
  //  ├─────────────────────────────────────────────┤ ├──────────────────────────────────────────────┤
         KC_F6,   KC_F7,   KC_F8,  KC_F9,   KC_F10,     _______, _______, _______, _______, _______,
  //  ├─────────────────────────────────────────────┤ ├──────────────────────────────────────────────┤
         KC_ENT,  KC_SLSH, KC_GRV, KC_F11,  KC_F12,     _______, _______, _______, _______, _______,
  //  ╰─────────────────────────────────────────────┤ ├──────────────────────────────────────────────╯
                        TG(_GMG), XXXXXXX, XXXXXXX,     _______, _______, _______
  //                  ╰─────────────────────────────╯ ╰───────────────────────────╯
  ),
  //}}}*/

  // Keypad {{{
  [_RKEYPAD] = LAYOUT(
  //  ╭──────────────────────────────────────────────╮ ╭─────────────────────────────────────────────╮
        _______, _______, _______, _______, _______,     KC_PPLS,   KC_P7,   KC_P8,  KC_P9, KC_PMNS,
  //  ├──────────────────────────────────────────────┤ ├─────────────────────────────────────────────┤
        _______, _______, _______, _______, _______,     KC_PAST,   KC_P4,   KC_P5,  KC_P6, KC_PSLS,
  //  ├──────────────────────────────────────────────┤ ├─────────────────────────────────────────────┤
        _______, _______, _______, _______, _______,     KC_PDOT,   KC_P1,   KC_P2,  KC_P3, KC_PENT,
  //  ╰──────────────────────────────────────────────┤ ├─────────────────────────────────────────────╯
                          _______, _______, _______,     KC_NUM, KC_BSPC, TO(_DEFAULT)
  //                    ╰────────────────────────────╯ ╰───────────────────────────────╯
  ),
  [_LKEYPAD] = LAYOUT(
  //  ╭─────────────────────────────────────────────╮ ╭─────────────────────────────────────────────╮
        KC_PPLS,   KC_P7,   KC_P8,  KC_P9, KC_PMNS,     _______, _______, _______, _______, _______,
  //  ├─────────────────────────────────────────────┤ ├─────────────────────────────────────────────┤
        KC_PAST,   KC_P4,   KC_P5,  KC_P6, KC_PSLS,     _______, _______, _______, _______, _______,
  //  ├─────────────────────────────────────────────┤ ├─────────────────────────────────────────────┤
        KC_PDOT,   KC_P1,   KC_P2,  KC_P3, KC_PENT,     _______, _______, _______, _______, _______,
  //  ╰─────────────────────────────────────────────┤ ├─────────────────────────────────────────────╯
                     TO(_DEFAULT), KC_BSPC, KC_NUM,     _______, _______, _______
  //               ╰────────────────────────────────╯ ╰───────────────────────────╯
  ),
  //}}}

  [_NAV] = LAYOUT(
  // ╭────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────╮
       TG(_DEFAULT), LCS_TAB,  KC_UP, LCTL_TAB, LCTL_PLUS,     KC_MUTE, KC_VOLD, KC_VOLU, XXXXXXX, XXXXXXX,
  // ├────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────┤
         TB_SCROLL, KC_LEFT, KC_DOWN, KC_RGHT, LCTL_MINS,     KC_HOME, KC_PGDN, KC_PGUP, KC_END,  KC_ESC,
  // ├────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────┤
           XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,   XXXXXXX,     KC_MSTP, KC_MPRV, KC_MNXT, KC_MPLY, KC_PSCR,
  // ╰────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────╯
                               MS_BTN3, MS_BTN1, MS_BTN2,     KC_LGUI, KC_LALT, TO(_DEFAULT)
  //                        ╰─────────────────────────────╯ ╰────────────────────────────────╯
  ),

  [_MOUSE] = LAYOUT(
  //  ╭──────────────────────────────────────────────╮ ╭──────────────────────────────────────────────╮
        KC_LALT, MS_BTN4, MS_BTN5, MS_BTN6, MS_BTN7,     MS_BTN7, MS_BTN6, MS_BTN5, MS_BTN4, KC_RALT,
  //  ├──────────────────────────────────────────────┤ ├──────────────────────────────────────────────┤
        KC_LCTL, MS_BTN3, MS_BTN2, MS_BTN1, MS_BTN8,     MS_BTN8, MS_BTN1, MS_BTN2, MS_BTN3, KC_RCTL,
  //  ├──────────────────────────────────────────────┤ ├──────────────────────────────────────────────┤
        KC_LSFT, MS_WHLL, MS_WHLU, MS_WHLD, MS_WHLR,     MS_WHLL, MS_WHLD, MS_WHLU, MS_WHLR, KC_RSFT,
  //  ╰──────────────────────────────────────────────┤ ├──────────────────────────────────────────────╯
                   TO(_DEFAULT), TB_SCROLL, _______,     _______, TB_SCROLL, TO(_DEFAULT)
  //             ╰───────────────────────────────────╯ ╰──────────────────────────────────╯
  ),

  [_FN] = LAYOUT(
  //       ╭──────────────────────────────────────────╮ ╭─────────────────────────────────────────╮
              KC_F1,  KC_F2,  KC_F3,  KC_F4,   KC_F5,     KC_F6,  KC_F7,  KC_F8,  KC_F9,  KC_F10,
  //       ├──────────────────────────────────────────┤ ├─────────────────────────────────────────┤
             KC_F11, KC_F12, KC_F13,  KC_F14, KC_F15,     KC_F16, KC_F17, KC_F18, KC_F19, KC_F20,
  //       ├──────────────────────────────────────────┤ ├─────────────────────────────────────────┤
             KC_LCTL, KC_DEL, KC_END, KC_F21, KC_F22,     KC_F23, KC_F24, KC_END, KC_DEL, KC_ENT,
  //       ╰──────────────────────────────────────────┤ ├─────────────────────────────────────────╯
                            KC_LALT, KC_LSFT, KC_END,     XXXX, KC_RSFT, TO(_DEFAULT)
  //                      ╰───────────────────────────╯ ╰─────────────────────────────╯
  ),

  [_QKC] = LAYOUT(
  // ╭──────────────────────────────────────────────────╮ ╭─────────────────────────────────────────────╮
       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     XXXXXXX,     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  // ├──────────────────────────────────────────────────┤ ├─────────────────────────────────────────────┤
       DB_TOGG, EE_CLR,  QK_MAKE, QK_BOOT,   QK_REBOOT,     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  // ├──────────────────────────────────────────────────┤ ├─────────────────────────────────────────────┤
       TG(_LAYERS), CL_TOGG, KO_TOGG, XXXXXXX, XXXXXXX,     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  // ╰──────────────────────────────────────────────────┤ ├─────────────────────────────────────────────╯
                             XXXXXXX, XXXXXXX, XXXXXXX,     XXXXXXX, XXXXXXX, TO(_DEFAULT)
  //                       ╰────────────────────────────╯ ╰────────────────────────────────╯
  ),

  [_LAYERS] = LAYOUT(
  //  ╭─────────────────────────────────────────╮ ╭─────────────────────────────────────────╮
         TG(0),  TG(1),  TG(2),  TG(3),  TG(4),     TG(5),  TG(6),  TG(7),  TG(8),  TG(9),
  //  ├─────────────────────────────────────────┤ ├─────────────────────────────────────────┤
        TG(10), TG(11), TG(12), TG(13), TG(14),     TG(15), TG(16), TG(17), TG(18), TG(19),
  //  ├─────────────────────────────────────────┤ ├─────────────────────────────────────────┤
        TG(20), TG(21), TG(22), TG(23), TG(24),     TG(25), TG(26), TG(27), TG(28), TG(29),
  //  ╰─────────────────────────────────────────┤ ├─────────────────────────────────────────╯
                        TG(30), TG(31), TG(32),     XXXXXXX, LAYER_TO, TO(_DEFAULT)
  //                  ╰─────────────────────────╯ ╰─────────────────────────────────╯
  )
};
// clang-format on
