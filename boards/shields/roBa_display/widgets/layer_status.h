/*
 * Copyright (c) 2026 MoriKei0314
 *
 * SPDX-License-Identifier: MIT
 *
 * Based on englmaxi/zmk-dongle-display (MIT).
 */

#pragma once

#include <lvgl.h>
#include <zephyr/kernel.h>

struct zmk_widget_roba_layer_status {
    sys_snode_t node;
    lv_obj_t *obj;
};

int zmk_widget_roba_layer_status_init(struct zmk_widget_roba_layer_status *widget, lv_obj_t *parent);
lv_obj_t *zmk_widget_roba_layer_status_obj(struct zmk_widget_roba_layer_status *widget);
