# WiFi Driver Sample Application

## Description

The WiFi Driver sample application demonstrates WiFi communication and network operations on the supported boards for this application. It performs comprehensive WiFi testing including SDIO-based WiFi bring-up, shell task creation, and network connectivity validation to ensure reliable wireless communication.

The sample includes multiple WiFi operations:
- **SDIO initialization:** Initialize SDIO1 interface for WiFi module communication.
- **WiFi module bring-up:** Initialize and configure WLAN chip modules (4612 or 43711) using prebuilt WiFi libraries.
- **Shell task creation:** Create WiFi shell tasks for test and control flows.
- **Network connectivity:** Establish WiFi connections and validate network operations.
- **Diagnostic logging:** Provide comprehensive status and diagnostic information through logger output.

During each run, the app logs initialization status, WiFi module configuration, network connection progress, and diagnostic results. This makes it easy for end users to confirm that WiFi setup and network operations are working as expected.

The latest example structure uses a **common application source tree** with board-specific hardware setup kept under `hw/<BOARD>/`. For this app:
- Common application sources such as `main.c`, `wifi_sample_app.c`, and `wifi_sample_app.h` stay in the app root.
- Application defconfigs are stored under `configs/`.
- Board and hardware-specific setup is selected from `hw/<BOARD>/`, for example `hw/SR110_RDK/`.

The application can also be exported and built as a **standalone app repository**. In that flow, keep this app in its own directory, point `SRSDK_DIR` to the SDK root, and build from the app directory itself. For the full application workflow model, see [Astra MCU SDK User Guide](../../../docs/Astra_MCU_SDK_User_Guide.md).

## Supported Boards

This application supports:
- `SR110_RDK`

Select the defconfig that matches your target board, and the build system will pick the corresponding board-specific hardware setup from `hw/<BOARD>/`.

## Hardware Requirements
- Astra Machina Micro Kit (SR110)
- **WLAN chip module 4612 or 43711 attached to the board** (required)
- **UART bridge adapter** for UART1 logging (required)

## Prerequisites
- Choose **one** setup path:
  - **CLI**: [Setup and Install SDK using CLI](../../../docs/Astra_MCU_SDK_Setup_and_Install_CLI.md)
  - **VS Code**: [Setup and Install SDK using VS Code](../../../docs/Astra_MCU_SDK_Setup_and_Install_VsCode.md)

## Test Case Selection

Before building, choose the testcase defconfig that matches your target board.

You can:
- Select the required defconfig directly from the application's `configs/` directory.
- Run `make list_defconfigs` from the application directory to list all supported defconfigs.

## Building and Flashing the Example using VS Code

Use the VS Code flow described in the respective soc vscode guides and the VS Code Extension guide:
- [SR110 Build and Flash with VS Code](../../../docs/SR110/SR110_Build_and_Flash_with_VSCode.md)
- [Astra MCU SDK VS Code Extension User Guide](../../../docs/Astra_MCU_SDK_VSCode_Extension_User_Guide.md)

**Build (VS Code):**
1. Open **Build and Deploy** -> **Build Configurations**.
2. Select **wifi_sample_app** in the **Application** dropdown.
3. Build with **Build (SDK + App)** for the first build, or **Build App** for rebuilds.

**Flash (VS Code):**
1. Use **Image Conversion** to generate the flash image.
2. Use **Image Flashing** (SWD/JTAG) to flash the firmware image.

---

## Building and Flashing the Example using CLI

Use the CLI flow described in the respective build guide:
- [SR110 Build and Flash with CLI](../../../docs/SR110/SR110_Build_and_Flash_with_CLI.md)
- [Astra MCU SDK User Guide](../../../docs/Astra_MCU_SDK_User_Guide.md)

**Build (CLI):**
1. Build from the application directory itself:
   ```bash
   cd <sdk-root>/examples/driver_examples/wifi_sample_app
   export SRSDK_DIR=<sdk-root>
   make <app_defconfig> BUILD=SRSDK
   ```
2. For faster rebuilds when only app code changes, reuse the app-local installed SDK package:
   ```bash
   cd <sdk-root>/examples/driver_examples/wifi_sample_app
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

1. Press **RESET** on the board after flashing.
2. For logging output, click **SERIAL MONITOR** and connect to the **UART1 bridge** port.
   - Use UART bridge adapter connected to UART1 (GPIO 24/23) for logger output
   - The logger port is not guaranteed to be consistent across OSes. As a starting point:
     - **Windows:** try the lower-numbered COM port first.
     - **Linux/macOS:** try the higher-numbered port first.
   - If you do not see logs after a reset, switch to the other port.
3. WiFi sample logs appear in the logger window, including initialization status and diagnostic results.

**Expected Logs**

```
000000072.126:[WiFi] Joining AP

000000072.128:posix_rtos_init_semaphore, 806, 0x30011f78

000000072.231:mhd_wifi_join_specific, 1057, 5, 0000d099

000000072.243:mhd_wifi_join_specific, 1077, 0 0x0

000000074.183:event from idx 0, type 3, status 0, reason 0, flags 0

000000074.186:-- JOIN Event (type 3 status 0 reason 0)

000000074.189:event from idx 0, type 16, status 0, reason 0, flags 1

000000074.192:-- JOIN Event (type 16 status 0 reason 0)

000000074.225:event from idx 0, type 0, status 0, reason 0, flags 0

000000074.228:-- JOIN Event (type 0 status 0 reason 0)

000000074.231:event from idx 0, type 46, status 6, reason 0, flags 0

000000074.234:-- JOIN Event (type 46 status 262 reason 512)

000000074.237:[WiFi] Join successfully ! sta_is_link_up=1
 
```
