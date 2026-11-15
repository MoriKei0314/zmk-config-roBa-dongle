/*
 * Copyright (c) 2026 MoriKei0314
 *
 * SPDX-License-Identifier: MIT
 *
 * Based on englmaxi/zmk-dongle-display widgets/layer_status.c (MIT).
 * Differences from the original:
 *  - Large font (Montserrat 20) and full-width, center aligned: the
 *    layer name is the most prominent element of this screen.
 *  - If the layer has no name (the keymap layer has neither a
 *    display-name nor a label property, so zmk_keymap_layer_name()
 *    returns ""), the layer index number is shown instead of an
 *    empty string.
 */

#include <zephyr/kernel.h>

#include <zephyr/logging/log.h>
LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

#include <zmk/display.h>
#include <zmk/events/layer_state_changed.h>
#include <zmk/event_manager.h>
#include <zmk/keymap.h>

#include "layer_status.h"

static sys_slist_t widgets = SYS_SLIST_STATIC_INIT(&widgets);

struct layer_status_state {
    uint8_t index;
    const char *label;
};

static void set_layer_text(lv_obj_t *label, struct layer_status_state state) {
    if (state.label == NULL || state.label[0] == '\0') {
        lv_label_set_text_fmt(label, "%u", state.index);
    } else {
        lv_label_set_text(label, state.label);
    }
}

static void layer_status_update_cb(struct layer_status_state state) {
    struct zmk_widget_roba_layer_status *widget;
    SYS_SLIST_FOR_EACH_CONTAINER(&widgets, widget, node) { set_layer_text(widget->obj, state); }
}

static struct layer_status_state layer_status_get_state(const zmk_event_t *eh) {
    uint8_t index = zmk_keymap_highest_layer_active();
    return (struct layer_status_state){
        .index = index,
        .label = zmk_keymap_layer_name(index),
    };
}

ZMK_DISPLAY_WIDGET_LISTENER(widget_roba_layer_status, struct layer_status_state,
                            layer_status_update_cb, layer_status_get_state)

ZMK_SUBSCRIPTION(widget_roba_layer_status, zmk_layer_state_changed);

int zmk_widget_roba_layer_status_init(struct zmk_widget_roba_layer_status *widget,
                                      lv_obj_t *parent) {
    widget->obj = lv_label_create(parent);
    lv_obj_set_width(widget->obj, 128); /* full width of the 128x64 panel */
    lv_obj_set_style_text_font(widget->obj, &lv_font_montserrat_20, LV_PART_MAIN);
    lv_obj_set_style_text_align(widget->obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
    lv_label_set_long_mode(widget->obj, LV_LABEL_LONG_SCROLL_CIRCULAR);

    sys_slist_append(&widgets, &widget->node);

    widget_roba_layer_status_init();

    return 0;
}

lv_obj_t *zmk_widget_roba_layer_status_obj(struct zmk_widget_roba_layer_status *widget) {
    return widget->obj;
}
