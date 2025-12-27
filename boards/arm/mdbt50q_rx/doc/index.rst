.. _mdbt50q_rx:

RAYTAC MDBT50Q-RX
=================

Overview
--------

The RAYTAC MDBT50Q-RX is a USB dongle based on the nRF52840 SoC with
Adafruit UF2 bootloader pre-installed, designed for use as a Bluetooth Low Energy (BLE)
receiver/dongle for ZMK keyboards.

Hardware
--------

- nRF52840 ARM Cortex-M4F CPU
- 1 MB Flash memory
- 256 KB RAM
- IEEE 802.15.4, Bluetooth Low Energy, ANT, NFC support
- USB 2.0 Full Speed
- Adafruit UF2 Bootloader

Supported Features
------------------

- UART (logging)
- GPIO
- BLE with SoftDevice S140

Programming and Debugging
--------------------------

The MDBT50Q-RX comes pre-loaded with Adafruit's UF2 bootloader.
This allows for simple drag-and-drop firmware updates.

To enter bootloader mode:
1. Unplug the dongle from USB
2. Hold down the button
3. Plug the dongle back in while holding the button
4. The dongle will appear as a USB drive
5. Drag and drop the .uf2 firmware file

Alternatively, with nRF Connect Programmer or adafruit-nrfutil.
