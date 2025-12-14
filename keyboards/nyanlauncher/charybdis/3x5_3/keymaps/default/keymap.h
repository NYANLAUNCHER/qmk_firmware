// vim: filetype=c
#pragma once
#include QMK_KEYBOARD_H

// Custom Settings
#define INIT_CPI 0
//#define TAP_CODE_DELAY 25
#define OSL_TIMEOUT 300 // Holding OSL() will trigger TG() instead
#define TOGG_TIMEOUT 300 // Holding toggle keys like KO_TOGG will always enable them
#define LH_TIMEOUT 100 // will check for keypresses until time depletes
#define MT_DELAY 25 // mods activated using mod taps will stay activated
#define SCROLL_DIVISOR_H 10.0
#define SCROLL_DIVISOR_V 10.0

// Aliases
#define IS_MODKEY(x) IS_MODIFIER_KEYCODE(x)
#define XXXX XXXXXXX
#define ____ _______
#define LCTL_PLUS C(KC_PLUS)
#define LCTL_MINS C(KC_MINS)
#define LCTL_TAB  C(KC_TAB)
#define LCS_TAB C(S(KC_TAB))
// Mod-Tap keys
#define CTL_SPC CTL_T(KC_SPC)
#define SFT_BSPC SFT_T(KC_BSPC)

#define LAYOUT_TRNS LAYOUT( \
_______, _______, _______, _______, _______,   _______, _______, _______, _______, _______, \
_______, _______, _______, _______, _______,   _______, _______, _______, _______, _______, \
_______, _______, _______, _______, _______,   _______, _______, _______, _______, _______, \
                  _______, _______, _______,   _______, _______, _______ )

#define LAYOUT_NOOP LAYOUT( \
XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
                  XXXXXXX, XXXXXXX, XXXXXXX,   XXXXXXX, XXXXXXX, XXXXXXX )

#define _DVORAK_LAYOUT = LAYOUT(                                                                                \
//   ╭────────────────────────────────────────────╮ ╭────────────────────────────────────────────╮
        KC_SCLN,  KC_COMM,  KC_DOT,  KC_P,  KC_Y,     KC_F,    KC_G,    KC_C,    KC_R,    KC_L,    \
//   ├────────────────────────────────────────────┤ ├────────────────────────────────────────────┤
        KC_A,     KC_O,     KC_E,    KC_U,  KC_I,     KC_D,    KC_H,    KC_T,    KC_N,    KC_S,    \
//   ├────────────────────────────────────────────┤ ├────────────────────────────────────────────┤
        KC_SCLN,  KC_Q,     KC_J,    KC_K,  KC_X,     KC_B,    KC_M,    KC_W,    KC_V,    KC_Z,    \
//   ╰────────────────────────────────────────────┤ ├────────────────────────────────────────────╯
                       _______, _______, _______,     TT(_RAISE), _______, _______                 \
//                   ╰────────────────────────────╯ ╰──────────────────────────────╯
)
