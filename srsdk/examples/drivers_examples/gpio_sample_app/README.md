# GPIO Driver Sample Application

## Description

The GPIO Driver sample application demonstrates GPIO digital input/output operations and interrupt handling on the supported boards for this application. It performs comprehensive GPIO testing including pin configuration, I/O operations, port-level operations, and interrupt handling to ensure reliable GPIO functionality.

The sample includes multiple GPIO operations:
- **Pin configuration:** Set GPIO pin configurations and verify values by reading them back.
- **Pin-level I/O:** Write logic values to test pins and confirm readback values.
- **Port-level I/O:** Set direction and write/read at port level using masks.
- **Pin interrupt handling:** Enable interrupts on test pins and verify ISR handling.
- **Port interrupt configuration:** Configure interrupt mask/type/polarity/both-edge/enable at port level and verify status.

During each run, the app logs configuration status, I/O operations, interrupt handling, and validation results. This makes it easy for end users to confirm that GPIO setup and operations are working as expected.

The latest example structure uses a **common application source tree** with board-specific hardware setup kept under `hw/<BOARD>/`. For this app:
- Common application sources such as `main.c`, `gpio_sample_app.c`, and `gpio_sample_app.h` stay in the app root.
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

## Test Case Selection

Before building, choose the testcase defconfig that matches your target board.

You can:
- Select the required defconfig directly from the application's `configs/` directory.
- Run `make list_defconfigs` from the application directory to list all supported defconfigs.

**Available defconfigs:**
- `sr110_rdk_cm55_gpio_sample_app_defconfig`


## Building and Flashing the Example using VS Code

Use the VS Code flow described in the respective soc vscode guides and the VS Code Extension guide:
- [SR110 Build and Flash with VS Code](../../../docs/SR110/SR110_Build_and_Flash_with_VSCode.md)
- [Astra MCU SDK VS Code Extension User Guide](../../../docs/Astra_MCU_SDK_VSCode_Extension_User_Guide.md)

**Build (VS Code):**
1. Open **Build and Deploy** -> **Build Configurations**.
2. Select **gpio_sample_app** in the **Application** dropdown.
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
   cd <sdk-root>/examples/driver_examples/gpio_sample_app
   export SRSDK_DIR=<sdk-root>
   make <app_defconfig> BUILD=SRSDK
   ```
2. For faster rebuilds when only app code changes, reuse the app-local installed SDK package:
   ```bash
   cd <sdk-root>/examples/driver_examples/gpio_sample_app
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
2. For logging output, click **SERIAL MONITOR** and connect to the **DAP logger** port on J14.
   - To make it easier to identify, ensure **only J14** is plugged in (not J13).
   - The logger port is not guaranteed to be consistent across OSes. As a starting point:
     - **Windows:** try the lower-numbered J14 COM port first.
     - **Linux/macOS:** try the higher-numbered J14 port first.
   - If you do not see logs after a reset, switch to the other J14 port.
3. GPIO sample logs appear in the logger window, including pass/fail status for each GPIO test step.

**Expected Logs**

```
0391469191:[0][WRN][LOGR]:Changing logger interface to LOGGER_IF_UART_1
0000000023:[0][INF][SYS ]:Application drivers initialization complete without errors.
0000004209:[0][INF][SYS ]:sr110 SDK version 1.3.0
0000006689:[0][INF][GPIO]:Starting GPIO Sample App!
0000009251:[0][INF][GPIO]:Testing GPIO pin configuration: start
0000012380:[0][INF][GPIO]:Testing GPIO pin configuration: success
0000015603:[0][INF][GPIO]:Testing GPIO pin I/O: start
0000018262:[0][INF][GPIO]:Pin 0 read back value for port 0: 1
0000021292:[0][INF][GPIO]:Pin 1 read back value for port 0: 1
0000024322:[0][INF][GPIO]:Pin 2 read back value for port 0: 1
0000027352:[0][INF][GPIO]:Pin 3 read back value for port 0: 1
0000030385:[0][INF][GPIO]:Testing GPIO pin I/O: success
0000033125:[0][INF][GPIO]:Testing GPIO port operations: start
0000036144:[0][INF][GPIO]:Direction mask for Port 0 : 0xF
0000038988:[0][INF][GPIO]:Port read value for Port 0 : 0xF
0000041868:[0][INF][GPIO]:Testing GPIO port operations: success
0000044991:[0][INF][GPIO]:Testing GPIO interrupts: start
0000047789:[0][INF][GPIO]:Interrupt handled and flag set to: 1
0000051683:[0][INF][GPIO]:Testing GPIO interrupts: success
0000051710:[0][INF][GPIO]:Testing GPIO port interrupt config: start
0000051736:[0][INF][GPIO]:Interrupt mask for Port 0 : 0x100000
0000051762:[0][INF][GPIO]:Interrupt type mask for Port 0 : 0x100000
0000051788:[0][INF][GPIO]:Both edge mask for Port 0 : 0x100000
0000051813:[0][INF][GPIO]:Polarity mask for Port 0 : 0x100000
0000051839:[0][INF][GPIO]:Interrupt enable mask for Port 0 : 0x100000
0000051865:[0][INF][GPIO]:Interrupt handled and flag set to: 1
0000052683:[0][INF][GPIO]:Testing GPIO port interrupt config: success
0000052718:[0][INF][GPIO]:GPIO Sample App Completed!
```
