/*
 * Copyright (c) 2026 MoriKei0314
 *
 * SPDX-License-Identifier: MIT
 *
 * Custom status screen for the roBa dongle (128x64, monochrome).
 * Layout:
 *
 *   y=0..18    [output status: USB/BT]        [L  95%]
 *                                             [R  43%]
 *   y=21       [layer name, Montserrat 16, centered, full width]
 *
 *   y=38..64   [Bongo Cat, 50x26]             [modifiers, small text]
 *
 * The battery rows are always visible; a side that has not reported
 * yet or is disconnected shows "L ---" / "R ---".
 *
 * The Bongo Cat (from the englmaxi module, WPM-animated) returned on
 * 2026-11-17 at the user's request ("not cute enough" without it).
 * It sits bottom-left; the modifiers moved to bottom-right, and the
 * layer font went 20 -> 16 so every element keeps its own band:
 * 18 (top rows) + 16 (layer) + 26 (cat) = 60 of 64 px, no overlap.
 *
 * Based on englmaxi/zmk-dongle-display (MIT).
 */

#include "custom_status_screen.h"
#include "widgets/battery_status.h"
#include "widgets/bongo_cat.h"
#include "widgets/layer_status.h"
#include "widgets/modifiers.h"
#include "widgets/output_status.h"

#include <zephyr/logging/log.h>
LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

static struct zmk_widget_output_status output_status_widget;

#if IS_ENABLED(CONFIG_ZMK_BATTERY)
static struct zmk_widget_roba_battery_status battery_status_widget;
#endif

#if IS_ENABLED(CONFIG_ZMK_ROBA_DISPLAY_BONGO_CAT)
static struct zmk_widget_bongo_cat bongo_cat_widget;
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
    /* Center band: layer name, large (Montserrat 16). */
    zmk_widget_roba_layer_status_init(&layer_status_widget, screen);
    lv_obj_align(zmk_widget_roba_layer_status_obj(&layer_status_widget), LV_ALIGN_TOP_LEFT, 0, 21);
#endif

#if IS_ENABLED(CONFIG_ZMK_ROBA_DISPLAY_BONGO_CAT)
    /* Bottom-left: Bongo Cat (50x26, WPM-animated). */
    zmk_widget_bongo_cat_init(&bongo_cat_widget, screen);
    lv_obj_align(zmk_widget_bongo_cat_obj(&bongo_cat_widget), LV_ALIGN_BOTTOM_LEFT, 2, 0);
#endif

#if IS_ENABLED(CONFIG_ZMK_ROBA_DISPLAY_MODIFIERS)
    /* Bottom-right: held modifiers as small text (moved from
     * bottom-left to make room for the cat). */
    zmk_widget_roba_modifiers_init(&modifiers_widget, screen);
    lv_obj_align(zmk_widget_roba_modifiers_obj(&modifiers_widget), LV_ALIGN_BOTTOM_RIGHT, 0, 0);
#endif

    return screen;
}
