/*
 * Copyright (c) 2026 MoriKei0314
 *
 * SPDX-License-Identifier: MIT
 *
 * Custom status screen for the roBa dongle (128x64, monochrome).
 * Layout (clarity first):
 *
 *   y=0        [output status: USB/BT]        [L  95%]
 *                                             [R  43%]
 *   y=26       [layer name, Montserrat 20, centered, full width]
 *
 *   y=56       [modifiers, small text]
 *
 * The battery rows are always visible; a side that has not reported
 * yet or is disconnected shows "L ---" / "R ---".
 *
 * There is intentionally no decorative image widget (the englmaxi
 * module's Bongo Cat) in this version: the space is spent on legible
 * battery and layer readouts. To add an image later, create a widget
 * under widgets/ and place it in the free area (e.g. bottom-right)
 * from zmk_display_status_screen() below.
 *
 * Based on englmaxi/zmk-dongle-display (MIT).
 */

#include "custom_status_screen.h"
#include "widgets/battery_status.h"
#include "widgets/layer_status.h"
#include "widgets/modifiers.h"
#include "widgets/output_status.h"

#include <zephyr/logging/log.h>
LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

static struct zmk_widget_output_status output_status_widget;

#if IS_ENABLED(CONFIG_ZMK_BATTERY)
static struct zmk_widget_roba_battery_status battery_status_widget;
#endif

#if IS_ENABLED(CONFIG_ZMK_ROBA_DISPLAY_LAYER)
static struct zmk_widget_roba_layer_status layer_status_widget;
#endif

#if IS_ENABLED(CONFIG_ZMK_ROBA_DISPLAY_MODIFIERS)
static struct zmk_widget_roba_modifiers modifiers_widget;
#endif

static lv_style_t global_style;

lv_obj_t *zmk_display_status_screen() {
    lv_obj_t *screen = lv_obj_create(NULL);

    lv_style_init(&global_style);
    lv_style_set_bg_color(&global_style, lv_color_white());
    lv_style_set_bg_opa(&global_style, LV_OPA_COVER);
    lv_style_set_text_color(&global_style, lv_color_black());
    lv_style_set_text_font(&global_style, &lv_font_unscii_8);
    lv_style_set_text_letter_space(&global_style, 1);
    lv_style_set_text_line_space(&global_style, 1);
    lv_obj_add_style(screen, &global_style, LV_PART_MAIN);

    /* Top-left: output / connection (USB or BT) indicator. */
    zmk_widget_output_status_init(&output_status_widget, screen);
    lv_obj_align(zmk_widget_output_status_obj(&output_status_widget), LV_ALIGN_TOP_LEFT, 0, 0);

#if IS_ENABLED(CONFIG_ZMK_BATTERY)
    /* Top-right: peripheral batteries, two rows "L 95%" / "R 43%". */
    zmk_widget_roba_battery_status_init(&battery_status_widget, screen);
    lv_obj_align(zmk_widget_roba_battery_status_obj(&battery_status_widget), LV_ALIGN_TOP_RIGHT, 0,
                 0);
#endif

#if IS_ENABLED(CONFIG_ZMK_ROBA_DISPLAY_LAYER)
    /* Center: layer name, large. */
    zmk_widget_roba_layer_status_init(&layer_status_widget, screen);
    lv_obj_align(zmk_widget_roba_layer_status_obj(&layer_status_widget), LV_ALIGN_TOP_LEFT, 0, 26);
#endif

#if IS_ENABLED(CONFIG_ZMK_ROBA_DISPLAY_MODIFIERS)
    /* Bottom-left: held modifiers as small text. */
    zmk_widget_roba_modifiers_init(&modifiers_widget, screen);
    lv_obj_align(zmk_widget_roba_modifiers_obj(&modifiers_widget), LV_ALIGN_BOTTOM_LEFT, 0, 0);
#endif

    return screen;
}
