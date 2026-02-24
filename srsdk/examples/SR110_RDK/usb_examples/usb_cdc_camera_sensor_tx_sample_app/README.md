# USB CDC Camera Sensor TX Sample App

## Description

This application facilitates communication with an Astra Machina Micro Eval Kit over a **USB CDC (Communication Device Class) serial port** to receive image data and object detection results. It can capture raw image streams, convert them into viewable image files, and visualize detected objects with bounding boxes and confidence scores.

## Prerequisites
- Choose **one** setup path:
  - **CLI**: [Setup and Install SDK using CLI](../../../../docs/Setup_and_Install_SDK_using_CLI.md)
  - **VS Code**: [Setup and Install SDK using VS Code](../../../../docs/Setup_and_Install_SDK_using_VSCode.md)
- Install required Python packages for `parser.py`:
  - `pyserial`
  - `Pillow`
  - `matplotlib`
  - `numpy`
  - `opencv-python`

```bash
pip install pyserial Pillow matplotlib numpy opencv-python
```

## Building and Flashing the Example using VS Code

Use the VS Code flow described in the SR110 guide and the VS Code Extension guide:
- [SR110 Build and Flash with VS Code](../../../../docs/SR110/SR110_Build_and_Flash_with_VSCode.md)
- [Astra MCU SDK VS Code Extension User Guide](../../../../docs/Astra_MCU_SDK_VSCode_Extension_User_Guide.md)

**Build (VS Code):**
1. Open **Build and Deploy** -> **Build Configurations**.
2. Select **usb_cdc_camera_sensor_tx_sample_app** in the **Application** dropdown.
3. Build with **Build (SDK + App)** for the first build, or **Build App** for rebuilds.

**Flash (VS Code):**
1. Use **Image Conversion** to generate the flash image.
2. Use **Image Flashing** (SWD/JTAG) to flash the firmware image.

---

## Building and Flashing the Example using CLI

Use the CLI flow described in the SR110 guide:
- [SR110 Build and Flash with CLI](../../../../docs/SR110/SR110_Build_and_Flash_with_CLI.md)

**Build (CLI):**
1. From `<sdk-root>/examples`, build the example:
   ```bash
   cd <sdk-root>/examples
   export SRSDK_DIR=<sdk-root>
   make cm55_usb_cdc_camera_sensor_tx_sample_app_defconfig BOARD=SR110_RDK BUILD=SRSDK
   ```

**Flash (CLI):**
1. Activate the SDK venv (required for image generation tools):
   ```bash
   # Linux/macOS
   source <sdk-root>/.venv/bin/activate
   # Windows PowerShell
   .\.venv\Scripts\Activate.ps1
   ```
2. Generate the flash image:
   ```bash
   cd <sdk-root>/tools/srsdk_image_generator
   python srsdk_image_generator.py \
     -B0 \
     -flash_image \
     -sdk_secured \
     -spk "<sdk-root>/tools/srsdk_image_generator/B0_Input_examples/spk_rc4_1_0_secure_otpk.bin" \
     -apbl "<sdk-root>/tools/srsdk_image_generator/B0_Input_examples/sr100_b0_bootloader_ver_0x012F_ASIC.axf" \
     -m55_image "<sdk-root>/examples/out/sr110_cm55_fw/release/sr110_cm55_fw.elf" \
     -flash_type "GD25LE128" \
     -flash_freq "67"
   ```
3. Flash the firmware image:
   ```bash
   cd <sdk-root>
   python tools/openocd/scripts/flash_xspi_tcl.py \
     --cfg_path tools/openocd/configs/sr110_m55.cfg \
     --image tools/srsdk_image_generator/Output/B0_Flash/B0_flash_full_image_GD25LE128_67Mhz_secured.bin \
     --erase-all
   ```

---

## Running the Application using VS Code Extension

1. Connect a USB cable to the application USB port on the SR110 board and press **RESET**.
2. For logs, open **SERIAL MONITOR** and connect to the DAP logger port.
3. On Windows, if required, update the COM port driver in Device Manager to **USB Serial Device**.
4. Run the parser script and listen on the USB CDC COM port:
   ```bash
   python parser.py -c <COM_PORT> -b <BAUDRATE> -s
   ```

## Features of Python Script

- **List Available Connections:** Discover and display connected USB CDC serial ports.
- **Receive Image Data:** Capture raw image data streamed from the device.
- **Save Images:** Convert received raw image data into `.tif` files.
- **Real-time Image Display:** Display incoming images in a pop-up window.
- **Visualize Detections:** Overlay bounding boxes and confidence scores.
- **Customizable Settings:** Configure COM port, baud rate, and output filename via CLI options.
