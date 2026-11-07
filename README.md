# zmk-config-roBa-dongle

roBa (43 keys, split, right-hand PMW3610 trackball) with a USB dongle
as the BLE central — 3 controllers total.

| Firmware | Board | Shield | Role |
|---|---|---|---|
| `roBa_L` | xiao_ble (Seeed XIAO BLE) | roBa_L | Left peripheral (encoder) |
| `roBa_R` | xiao_ble (Seeed XIAO BLE) | roBa_R | Right peripheral (trackball) |
| `roBa_dongle` | nice_nano (Nice!Nano v2) | roBa_dongle + dongle_display | Central: keymap, ZMK Studio, OLED |
| `settings_reset-*` | xiao_ble / nice_nano | settings_reset | Bonding/settings reset |

Keymap and ZMK Studio live on the dongle (the central). Flash all
three main firmwares together when the keymap or device tree changes;
flash `settings_reset` to all three and re-pair if bonding breaks.

<img src="keymap-drawer/roBa.svg" >
