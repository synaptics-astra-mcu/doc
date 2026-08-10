# USB CDC Camera Sensor TX Sample App

## Description

The USB CDC Camera Sensor TX Sample application demonstrates USB CDC communication and image data transmission on the supported boards for this application. It performs comprehensive USB CDC testing including camera sensor integration, image data streaming, and object detection visualization to ensure reliable USB-based image transfer and processing.

The sample includes multiple USB CDC operations:
- **USB CDC initialization:** Initialize USB CDC serial communication with host PC.
- **Camera sensor integration:** Interface with camera sensors for image capture.
- **Image data streaming:** Transmit raw image data over USB CDC connection.
- **Object detection processing:** Process images for object detection with confidence scoring.
- **Bounding box visualization:** Generate visual representations of detected objects.
- **Image conversion:** Convert raw image streams to viewable image files.
- **Host communication:** Maintain reliable communication with Astra Machina Micro Eval Kit.

During each run, the app logs initialization status, image capture progress, detection results, and transmission statistics. This makes it easy for end users to confirm that USB CDC setup and image operations are working as expected.

The latest example structure uses a **common application source tree** with board-specific hardware setup kept under `hw/<BOARD>/`. For this app:
- Common application sources such as `main.c` and camera-related files stay in the app root.
- Application defconfigs are stored under `configs/`.
- Board and hardware-specific setup is selected from `hw/<BOARD>/`, for example `hw/SR110_RDK/`.

The application can also be exported and built as a **standalone app repository**. In that flow, keep this app in its own directory, point `SRSDK_DIR` to the SDK root, and build from the app directory itself. For the full application workflow model, see [Astra MCU SDK User Guide](../../../docs/Astra_MCU_SDK_User_Guide.md).

## Supported Boards

This application supports:
- `SR110_RDK`

Select the defconfig that matches your target board, and the build system will pick the corresponding board-specific hardware setup from `hw/<BOARD>/`.

## Prerequisites
- Choose **one** setup path:
  - **CLI**: [Setup and Install SDK using CLI](../../../docs/Astra_MCU_SDK_Setup_and_Install_CLI.md)
  - **VS Code**: [Setup and Install SDK using VS Code](../../../docs/Astra_MCU_SDK_Setup_and_Install_VsCode.md)
- Install required Python packages for `parser.py`:
  - `pyserial`
  - `Pillow`
  - `matplotlib`
  - `numpy`
  - `opencv-python`

```bash
pip install pyserial Pillow matplotlib numpy opencv-python
```

## Project Configuration Selection

Before building, choose the project configuration (defconfig) that matches your target board.

You can:
- Select the required defconfig directly from the application's `configs/` directory.
- Run `make list_defconfigs` from the application directory to list all supported defconfigs.

**Available defconfigs:**
- `sr110_rdk_cm55_usb_cdc_camera_sensor_tx_sample_app_defconfig`


## Building and Flashing the Example using VS Code and CLI

Use the VS Code flow described in the SR110 guide and the VS Code Extension guide:
- [SR110 Build and Flash with VS Code](../../../docs/SR110/SR110_Build_and_Flash_with_VSCode.md)
- [Astra MCU SDK VS Code Extension User Guide](../../../docs/Astra_MCU_SDK_VSCode_Extension_User_Guide.md)

**Build (VS Code):**
1. Open **Build and Deploy** -> **Build Configurations**.
2. Select the **usb_cdc_camera_sensor_tx_sample_app** project configuration in the **Project Configuration** dropdown.
3. Build with **Build (SDK+Project)** for the first build, or **Build (Project)** for rebuilds.

**Build (CLI):**
1. Build from the application directory itself:
   ```bash
   cd <sdk-root>/examples/usb_examples/usb_cdc_camera_sensor_tx_sample_app
   export SRSDK_DIR=<sdk-root>
   make <app_defconfig> BUILD=SRSDK
   ```
2. For faster rebuilds when only app code changes, reuse the app-local installed SDK package:
   ```bash
   cd <sdk-root>/examples/usb_examples/usb_cdc_camera_sensor_tx_sample_app
   export SRSDK_DIR=<sdk-root>
   make build
   ```
3. If this app has been exported to its own repository, use the same commands from that exported app directory after setting `SRSDK_DIR` to the SDK root.

**Build outputs (CLI):**
- Application binary: `<app-dir>/out/<target>/release/<target>.elf`
- App-local SDK package: `<app-dir>/install/<BOARD>/<BUILD_TYPE>/`

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
     -spk "<sdk-root>/tools/srsdk_image_generator/Inputs/spk_rc4_1_0_secure_otpk.bin" \
     -apbl "<sdk-root>/tools/srsdk_image_generator/Inputs/sr100_b0_bootloader_ver_0x012F_ASIC.axf" \
     -m55_image "<sdk-root>/examples/usb_examples/usb_cdc_camera_sensor_tx_sample_app/out/sr110_cm55_fw/release/sr110_cm55_fw.elf" \
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
