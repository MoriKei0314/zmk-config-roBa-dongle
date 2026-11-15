/*
 * Copyright (c) 2026 MoriKei0314
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <lvgl.h>
#include <zephyr/kernel.h>

struct zmk_widget_roba_modifiers {
    sys_snode_t node;
    lv_obj_t *obj;
};

int zmk_widget_roba_modifiers_init(struct zmk_widget_roba_modifiers *widget, lv_obj_t *parent);
lv_obj_t *zmk_widget_roba_modifiers_obj(struct zmk_widget_roba_modifiers *widget);
