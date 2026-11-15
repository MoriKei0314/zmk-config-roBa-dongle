/*
 * Copyright (c) 2026 MoriKei0314
 *
 * SPDX-License-Identifier: MIT
 *
 * Minimal text modifiers indicator: shows the currently held modifiers
 * as plain text (e.g. "Ctrl Shift") in the small unscii-8 font, empty
 * when none are held. Unlike the englmaxi icon-based widget this needs
 * no symbol images, which keeps the 128x64 layout uncrowded.
 */

#include <stdio.h>
#include <zephyr/kernel.h>

#include <zephyr/logging/log.h>
LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

#include <dt-bindings/zmk/modifiers.h>
#include <zmk/display.h>
#include <zmk/events/modifiers_state_changed.h>
#include <zmk/event_manager.h>
#include <zmk/hid.h>

#include "modifiers.h"

static sys_slist_t widgets = SYS_SLIST_STATIC_INIT(&widgets);

struct modifiers_state {
    zmk_mod_flags_t modifiers;
};

static void set_modifiers_text(lv_obj_t *label, struct modifiers_state state) {
    char text[40];
    size_t pos = 0;

    text[0] = '\0';
    if (state.modifiers & (MOD_LCTL | MOD_RCTL)) {
        pos += snprintf(text + pos, sizeof(text) - pos, "Ctrl ");
    }
    if (state.modifiers & (MOD_LSFT | MOD_RSFT)) {
        pos += snprintf(text + pos, sizeof(text) - pos, "Shift ");
    }
    if (state.modifiers & (MOD_LALT | MOD_RALT)) {
        pos += snprintf(text + pos, sizeof(text) - pos, "Alt ");
    }
    if (state.modifiers & (MOD_LGUI | MOD_RGUI)) {
        pos += snprintf(text + pos, sizeof(text) - pos, "Gui ");
    }
    if (pos > 0) {
        text[pos - 1] = '\0'; /* drop the trailing space */
    }
    lv_label_set_text(label, text);
}

static void modifiers_update_cb(struct modifiers_state state) {
    struct zmk_widget_roba_modifiers *widget;
    SYS_SLIST_FOR_EACH_CONTAINER(&widgets, widget, node) { set_modifiers_text(widget->obj, state); }
}

static struct modifiers_state modifiers_get_state(const zmk_event_t *eh) {
    return (struct modifiers_state){
        .modifiers = zmk_hid_get_explicit_mods(),
    };
}

ZMK_DISPLAY_WIDGET_LISTENER(widget_roba_modifiers, struct modifiers_state, modifiers_update_cb,
                            modifiers_get_state)

ZMK_SUBSCRIPTION(widget_roba_modifiers, zmk_modifiers_state_changed);

int zmk_widget_roba_modifiers_init(struct zmk_widget_roba_modifiers *widget, lv_obj_t *parent) {
    widget->obj = lv_label_create(parent);
    lv_label_set_text(widget->obj, "");

    sys_slist_append(&widgets, &widget->node);

    widget_roba_modifiers_init();

    return 0;
}

lv_obj_t *zmk_widget_roba_modifiers_obj(struct zmk_widget_roba_modifiers *widget) {
    return widget->obj;
}
