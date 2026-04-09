# USB CDC Image Downloader Sample App

## Description

The USB CDC Image Downloader Sample application demonstrates USB CDC communication and image data transfer on the supported boards for this application. It performs comprehensive USB CDC testing including bidirectional image frame communication, raw image data conversion, and real-time image display to ensure reliable USB-based image transfer and processing.

The sample includes multiple USB CDC operations:
- **USB CDC initialization:** Initialize USB CDC serial communication with host PC.
- **Bidirectional communication:** Support two-way USB CDC communication for image frames.
- **Image data reception:** Receive raw image data from connected device.
- **Image conversion:** Convert raw image data to viewable image files.
- **Real-time display:** Optionally display images in real-time during reception.
- **Image transmission:** Send predefined raw image files on trigger sequence.
- **Frame detection:** Detect and process image frame boundaries and headers.

During each run, the app logs initialization status, communication progress, image transfer statistics, and conversion results. This makes it easy for end users to confirm that USB CDC setup and image operations are working as expected.

The latest example structure uses a **common application source tree** with board-specific hardware setup kept under `hw/<BOARD>/`. For this app:
- Common application sources such as `main.c` and image-related files stay in the app root.
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
- Install required Python packages for `detect_frame.py`:
  - `pyserial`
  - `Pillow`
  - `matplotlib`
  - `numpy`

```bash
pip install pyserial Pillow matplotlib numpy
```

## Test Case Selection

Before building, choose the testcase defconfig that matches your target board.

You can:
- Select the required defconfig directly from the application's `configs/` directory.
- Run `make list_defconfigs` from the application directory to list all supported defconfigs.

**Available defconfigs:**
- `sr110_rdk_cm55_usb_cdc_image_downloader_sample_app_defconfig`


## Building and Flashing the Example using VS Code

Use the VS Code flow described in the SR110 guide and the VS Code Extension guide:
- [SR110 Build and Flash with VS Code](../../../docs/SR110/SR110_Build_and_Flash_with_VSCode.md)
- [Astra MCU SDK VS Code Extension User Guide](../../../docs/Astra_MCU_SDK_VSCode_Extension_User_Guide.md)

**Build (VS Code):**
1. Open **Build and Deploy** -> **Build Configurations**.
2. Select **usb_cdc_image_downloader_sample_app** in the **Application** dropdown.
3. Build with **Build (SDK + App)** for the first build, or **Build App** for rebuilds.

**Flash (VS Code):**
1. Use **Image Conversion** to generate the flash image.
2. Use **Image Flashing** (SWD/JTAG) to flash the firmware image.

---

## Building and Flashing the Example using CLI

Use the CLI flow described in the SR110 guide:
- [SR110 Build and Flash with CLI](../../../docs/SR110/SR110_Build_and_Flash_with_CLI.md)

**Build (CLI):**
1. Build from the application directory itself:
   ```bash
   cd <sdk-root>/examples/usb_examples/usb_cdc_image_downloader_sample_app
   export SRSDK_DIR=<sdk-root>
   make <app_defconfig> BUILD=SRSDK
   ```
2. For faster rebuilds when only app code changes, reuse the app-local installed SDK package:
   ```bash
   cd <sdk-root>/examples/usb_examples/usb_cdc_image_downloader_sample_app
   export SRSDK_DIR=<sdk-root>
   make build
   ```
3. If this app has been exported to its own repository, use the same commands from that exported app directory after setting `SRSDK_DIR` to the SDK root.

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
     -m55_image "<sdk-root>/examples/<example_type>/<app>/out/sr110_cm55_fw/release/sr110_cm55_fw.elf" \
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
2. For logging output, click **SERIAL MONITOR** and connect to the **DAP logger** port on J14.
   - To make it easier to identify, ensure **only J14** is plugged in (not J13).
   - The logger port is not guaranteed to be consistent across OSes. As a starting point:
     - **Windows:** try the lower-numbered J14 COM port first.
     - **Linux/macOS:** try the higher-numbered J14 port first.
   - If you do not see logs after a reset, switch to the other J14 port.
3. On **Windows**, convert **SYNA CDC 1** to a serial COM port before running `detect_frame.py`:
   1. Open **Device Manager** and go to **Universal Serial Bus devices**.

      ![SYNA CDC 1 in Universal Serial Bus devices](assets/vs_syna_cdc_1.png)

   2. Right-click **SYNA CDC 1** and select **Update driver**.

      ![Right click SYNA CDC 1 and select Update driver](assets/vs_update_driver.png)

   3. Select **Browse my computer for drivers**.

      ![Select Browse my computer for drivers](assets/vs_browse.png)

   4. Select **Let me pick from a list of available drivers on my computer**.

      ![Select Let me pick from a list of available drivers on my computer](assets/vs_pick_available_drivers.png)

   5. Choose **USB Serial Device** and complete the installation.
      
      ![Select USB Serial Device from the available drivers](assets/vs_select_usb_serial_device.png)
   
   6. After the update, a new entry appears under **Ports (COM & LPT)** as **USB Serial Device (COMx)** (for example, `COM4`).
      
      ![USB Serial Device appears under Ports (COM & LPT)](assets/vs_device_mng_result.png)

      - Use that `COMx` value as `<COM_PORT>` in the script command.

4. Run the downloader script on that USB CDC COM port:
   ```bash
   python detect_frame.py -c <COM_PORT> -b <BAUD_RATE> -s
   ```

5. Expected behavior after running the script:
   - The board opens an image-download window and sends an `OPEN` trigger over USB CDC.
   - The host script detects `OPEN`, sends one bundled QVGA raw frame (`sample_app_frame_qvga_01.raw` and `sample_app_frame_qvga_03.raw` alternately), and saves the transmitted payload as `<file_name>_sent.raw`.
   - The board validates checksum and then streams the image back with the image tag.
   - The script receives that frame, saves `uart_recv.raw` and `uart_recv.raw.tif` (default), and with `-s` updates the Matplotlib image window.
   - This upload/download image cycle repeats continuously while the USB CDC connection remains active.
   
   ![Running USB CDC Image Downloader SAmple App](assets/vs_image_downloader_app.png)

## Features of Python Script

- **Serial Port Communication:** Opens and manages USB CDC serial connection.
- **Receive Raw Image Frames:** Detects frame tags and receives raw image payloads.
- **Image Processing and Saving:** Converts data to grayscale `.tif` output.
- **Real-time Image Display:** Optionally displays received frames.
- **Send Image Frames:** Sends predefined raw frames on trigger.
- **COM Port Listing:** Helps identify available serial ports.
