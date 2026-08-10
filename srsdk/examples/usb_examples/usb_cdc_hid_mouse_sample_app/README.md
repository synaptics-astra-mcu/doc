# USB CDC + HID Mouse Composite Sample App

This application demonstrates a **composite USB device** on SR110 that exposes:

- **USB CDC interfaces** for serial communication (echo behavior), and
- **USB HID mouse interface** that sends periodic mouse movement reports.

## Application Behavior

### CDC behavior

- Input CDC interface: `ITF 1`
- Output CDC interface(s): `ITF 0` and optionally `ITF 1`
- Incoming data is read from input CDC and echoed back with case conversion.

### HID mouse behavior

- Registers a HID mouse interface and sends mouse reports periodically.
- Moves the cursor in a square pattern.
- Sends reports every 10 ms.

## Build

```bash
cd <sdk-root>/examples/usb_examples/usb_cdc_hid_mouse_sample_app
export SRSDK_DIR=<sdk-root>
make sr110_rdk_cm55_usb_cdc_hid_mouse_sample_app_defconfig BUILD=SRSDK
```

## Expected Result

- HID mouse enumerates.
- CDC COM port(s) enumerate.
- Cursor movement is visible on host.
- CDC echo behavior works on serial terminal.
