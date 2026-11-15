/*
 * Copyright (c) 2026 MoriKei0314
 *
 * SPDX-License-Identifier: MIT
 *
 * Based on englmaxi/zmk-dongle-display widgets/battery_status.c (MIT).
 * Differences from the original:
 *  - Each row carries an explicit "L" / "R" label.
 *  - Rows are ALWAYS visible. A side that has not reported yet, or that
 *    disconnected (the central raises a level=0 event on disconnect),
 *    shows "L ---" / "R ---" instead of hiding the row.
 *  - The small battery canvas icon is dropped; text only, for clarity
 *    on the 128x64 panel.
 */

#include <zephyr/kernel.h>

#include <zephyr/logging/log.h>
LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

#include <zmk/ble.h>
#include <zmk/display.h>
#include <zmk/events/battery_state_changed.h>
#include <zmk/event_manager.h>

#include "battery_status.h"

/*
 * source と左右の対応付け（要・実機確認）
 *
 * zmk_peripheral_battery_state_changed.source は「BLE セントラルの接続
 * スロット番号」であって、左右を保証する値ではない。スロットは最初に
 * 発見・ボンディングされた順に割り当てられ、その対応は settings に
 * 保存されるため、通常の再起動では変わらず実質的には安定している。
 *
 * ここでは初期仮定として source 0 = 左 とする。実機で必ず一度確認する
 * こと: 片側だけ電源を切ったとき、"---" に変わる行がその側と一致する
 * かを見る。一致しない場合は下の値を 1 に変えてリビルドする。
 * settings リセットや再ペアリングを行うと対応が逆転することがある
 * ので、その際も同じ確認をやり直すこと。
 */
#define ROBA_BATTERY_SOURCE_LEFT 0

#ifndef ZMK_SPLIT_BLE_PERIPHERAL_COUNT
#define ZMK_SPLIT_BLE_PERIPHERAL_COUNT 0
#endif

/* This screen is built for exactly two peripherals (L and R). */
#if ZMK_SPLIT_BLE_PERIPHERAL_COUNT != 2
#warning "roBa_display battery widget assumes exactly 2 split peripherals"
#endif

#define ROBA_SIDE_LEFT 0
#define ROBA_SIDE_RIGHT 1

/* Row geometry: two rows of unscii-8 text (8 px font, 10 px pitch),
 * matching the layout in custom_status_screen.c. */
#define ROBA_BATTERY_ROW_HEIGHT 10

static sys_slist_t widgets = SYS_SLIST_STATIC_INIT(&widgets);

struct battery_state {
    uint8_t source;
    uint8_t level;
};

/* Latest known level per side; -1 = nothing received yet. */
static int16_t side_levels[2] = {-1, -1};

static lv_obj_t *side_labels[2];

static uint8_t side_for_source(uint8_t source) {
    return (source == ROBA_BATTERY_SOURCE_LEFT) ? ROBA_SIDE_LEFT : ROBA_SIDE_RIGHT;
}

static void render_side(uint8_t side) {
    lv_obj_t *label = side_labels[side];
    if (label == NULL) {
        return;
    }
    const char *side_name = (side == ROBA_SIDE_LEFT) ? "L" : "R";
    if (side_levels[side] < 0) {
        lv_label_set_text_fmt(label, "%s ---", side_name);
    } else {
        lv_label_set_text_fmt(label, "%s %3d%%", side_name, side_levels[side]);
    }
}

static void set_battery_state(struct battery_state state) {
    if (state.source >= ZMK_SPLIT_BLE_PERIPHERAL_COUNT) {
        return;
    }
    uint8_t side = side_for_source(state.source);
    /* The central emits level=0 when a peripheral disconnects
     * (split/bluetooth/central.c, split_central_disconnected), and a
     * real 0% report is indistinguishable from it. Both are shown as
     * "---" (unknown) rather than a bogus "0%". A later non-zero report
     * restores the percentage. */
    side_levels[side] = (state.level == 0) ? -1 : state.level;
    render_side(side);
}

static void battery_status_update_cb(struct battery_state state) {
    struct zmk_widget_roba_battery_status *widget;
    SYS_SLIST_FOR_EACH_CONTAINER(&widgets, widget, node) { set_battery_state(state); }
}

static struct battery_state battery_status_get_state(const zmk_event_t *eh) {
    const struct zmk_peripheral_battery_state_changed *ev =
        as_zmk_peripheral_battery_state_changed(eh);
    return (struct battery_state){
        .source = ev->source,
        .level = ev->state_of_charge,
    };
}

ZMK_DISPLAY_WIDGET_LISTENER(widget_roba_battery_status, struct battery_state,
                            battery_status_update_cb, battery_status_get_state)

ZMK_SUBSCRIPTION(widget_roba_battery_status, zmk_peripheral_battery_state_changed);

int zmk_widget_roba_battery_status_init(struct zmk_widget_roba_battery_status *widget,
                                        lv_obj_t *parent) {
    widget->obj = lv_obj_create(parent);
    lv_obj_set_size(widget->obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);

    for (int side = 0; side < 2; side++) {
        lv_obj_t *label = lv_label_create(widget->obj);
        lv_obj_align(label, LV_ALIGN_TOP_RIGHT, 0, side * ROBA_BATTERY_ROW_HEIGHT);
        side_labels[side] = label;
        render_side(side);
    }

    sys_slist_append(&widgets, &widget->node);

    widget_roba_battery_status_init();

    return 0;
}

lv_obj_t *zmk_widget_roba_battery_status_obj(struct zmk_widget_roba_battery_status *widget) {
    return widget->obj;
}
