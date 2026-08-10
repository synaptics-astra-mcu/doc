# USB HID Mouse Sample App

This application demonstrates a **USB HID mouse device** on SR110.

It enumerates as a standard HID mouse and periodically sends mouse movement reports to the host.

## Application Behavior

- Registers one HID mouse interface before USB initialization.
- Sends HID reports every 10 ms from a dedicated FreeRTOS task.
- Moves the pointer in a square pattern (right -> down -> left -> up).
- Uses simple 3-byte mouse reports: `buttons`, `x`, `y`.
- Supports remote wakeup when USB is suspended.

## Build

```bash
cd <sdk-root>/examples/usb_examples/usb_hid_mouse_sample_app
export SRSDK_DIR=<sdk-root>
make sr110_rdk_cm55_usb_hid_mouse_sample_app_defconfig BUILD=SRSDK
```

## Expected Result

- Host enumerates a HID mouse device.
- The laptop/host cursor moves automatically in a square pattern.

## Notes

- This sample is HID-only; CDC is disabled via `CONFIG_MODULE_USB_CDC_ENABLED=n`.
- For flashing and image generation flow, refer to the SDK user guide in `docs/`.
