# Pinmux Driver Sample Application

## Description

The Pinmux Driver sample application demonstrates pin multiplexing and electrical configuration on the supported boards for this application. It performs comprehensive pinmux testing including function selection, electrical parameter configuration, and validation to ensure reliable pin operation.

The sample includes multiple pinmux operations:
- **Full electrical configuration:** Set and read back complete electrical configuration for a pin.
- **Individual parameter configuration:** Set and read back individual electrical parameters.
- **Function mapping:** Configure and verify pin function selection (SPI/UART/GPIO).
- **Combined configuration:** Apply both function and electrical settings for single pins.
- **Multi-pin configuration:** Apply combined configurations to multiple pins simultaneously.

During each run, the app logs configuration status, readback verification, and validation results. This makes it easy for end users to confirm that pinmux setup and electrical configurations are working as expected.

The latest example structure uses a **common application source tree** with board-specific hardware setup kept under `hw/<BOARD>/`. For this app:
- Common application sources such as `main.c`, `pinmux_sample_app.c`, and `pinmux_sample_app.h` stay in the app root.
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
- `sr110_rdk_cm55_pinmux_sample_app_defconfig`


## Building and Flashing the Example using VS Code

Use the VS Code flow described in the respective soc vscode guides and the VS Code Extension guide:
- [SR110 Build and Flash with VS Code](../../../docs/SR110/SR110_Build_and_Flash_with_VSCode.md)
- [Astra MCU SDK VS Code Extension User Guide](../../../docs/Astra_MCU_SDK_VSCode_Extension_User_Guide.md)

**Build (VS Code):**
1. Open **Build and Deploy** -> **Build Configurations**.
2. Select the **pinmux_sample_app** project configuration in the **Project Configuration** dropdown.
3. Build with **Build (SDK+Project)** for the first build, or **Build (Project)** for rebuilds.

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
   cd <sdk-root>/examples/driver_examples/pinmux_sample_app
   export SRSDK_DIR=<sdk-root>
   make <app_defconfig> BUILD=SRSDK
   ```
2. For faster rebuilds when only app code changes, reuse the app-local installed SDK package:
   ```bash
   cd <sdk-root>/examples/driver_examples/pinmux_sample_app
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
     -m55_image "<sdk-root>/examples/driver_examples/pinmux_sample_app/out/sr110_cm55_fw/release/sr110_cm55_fw.elf" \
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
3. Pinmux sample logs appear in the logger window, including pass/fail status for pin function and electrical configuration checks.

**Expected Logs**

```
0391469361:[0][WRN][LOGR]:Changing logger interface to LOGGER_IF_UART_1
0000000023:[0][INF][SYS ]:Application drivers initialization complete without errors.
0000004205:[0][INF][SYS ]:sr110 SDK version 1.3.0
0000006674:[0][INF][PMUX]:Pinmux set pin electrical characteristics test start
0000010516:[0][INF][PMUX]:Drive strength: 0, Enable_input: 1, Enable_hold :0, Pull_state:0, slew_rate:1, schmitt_trigger:0
0000016467:[0][INF][PMUX]:Pinmux set pin electrical characteristics test end
0000020203:[0][INF][PMUX]:Pinmux set pin electrical parameter test start
0000023759:[0][INF][PMUX]:Driver strength: 3
0000025980:[0][INF][PMUX]:input enable: 1
0000028053:[0][INF][PMUX]:Hold enable: 1
0000030082:[0][INF][PMUX]:pull state: 0
0000032059:[0][INF][PMUX]:slew rate: 0
0000033993:[0][INF][PMUX]:schmitt trigger: 1
0000036207:[0][INF][PMUX]:Pinmux set pin electrical parameter test end
0000039665:[0][INF][PMUX]:Pinmux set pin function test start
0000042647:[0][INF][PMUX]:Pin function: 1
0000044722:[0][INF][PMUX]:Pinmux set pin function test end
0000047608:[0][INF][PMUX]:Pinmux configuring both electrical characteristics and function for a pin is successful
0000053121:[0][INF][PMUX]:Pinmux configuring both electrical characteristics and function for multiple pins is successful
0000059020:[0][INF][PMUX]:All Pinmux Driver APIs functionalities are verified successfully!
```
