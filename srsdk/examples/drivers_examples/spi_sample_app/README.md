# SPI Driver Sample Application

## Description

The SPI Driver sample application demonstrates SPI communication and data transfer operations on the supported boards for this application. It performs comprehensive SPI testing including controller/peripheral modes, transfer methods, and data integrity validation to ensure reliable SPI functionality.

The sample includes multiple SPI operations:
- **Single-board mode:** Controller and peripheral both run on the same board for loopback testing.
- **Double-board mode:** One board runs controller image, second board runs peripheral image for inter-board communication.
- **Transfer methods:** Blocking, non-blocking, and DMA-based transfer paths for different performance requirements.
- **Data validation:** Send known data patterns and verify received data matches on both controller and peripheral sides.

During each run, the app logs initialization status, transfer progress, timing information, and validation results. This makes it easy for end users to confirm that SPI setup and data operations are working as expected.

The latest example structure uses a **common application source tree** with board-specific hardware setup kept under `hw/<BOARD>/`. For this app:
- Common application sources such as `main.c`, `spi_sample_app.c`, and `spi_sample_app.h` stay in the app root.
- Application defconfigs are stored under `configs/`.
- Board and hardware-specific setup is selected from `hw/<BOARD>/`, for example `hw/SR110_RDK/`.

The application can also be exported and built as a **standalone app repository**. In that flow, keep this app in its own directory, point `SRSDK_DIR` to the SDK root, and build from the app directory itself. For the full application workflow model, see [Astra MCU SDK User Guide](../../../docs/Astra_MCU_SDK_User_Guide.md).

## Supported Boards

This application supports:
- `SR110_RDK`

Select the defconfig that matches your target board, and the build system will pick the corresponding board-specific hardware setup from `hw/<BOARD>/`.

## Hardware Requirements
- Astra Machina Micro Kit (SR110)
- UART bridge adapter (required for logging)
- SPI jumper wires
- For double-board mode: 2x Astra Machina Micro boards

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
- `sr110_rdk_cm55_spi_sample_app_double_board_controller_defconfig`
- `sr110_rdk_cm55_spi_sample_app_double_board_peripheral_defconfig`
- `sr110_rdk_cm55_spi_sample_app_preroll_controller_defconfig`
- `sr110_rdk_cm55_spi_sample_app_single_board_defconfig`

For this app, the default defconfig is:
   - `sr110_rdk_cm55_spi_sample_app_single_board_defconfig`

## Mode Configuration (Important)

Update `spi_sample_app.h` before build.

### Single-board non-blocking mode (requested baseline)
Set:
- `SPI_NON_BLOCKING_XFER = 1`
- `SPI_SINGLE_BOARD_MODE = 1`
- `SPI_DMA_XFER = 0`
- `SPI_DOUBLE_BOARD_MODE = 0`

### Single-board DMA mode
Set:
- `SPI_DMA_XFER = 1`
- Keep `SPI_SINGLE_BOARD_MODE = 1`
- Keep `SPI_DOUBLE_BOARD_MODE = 0`

### Double-board mode
Set:
- `SPI_DOUBLE_BOARD_MODE = 1`
- `SPI_SINGLE_BOARD_MODE = 0`
- Build two different images:
  - **Controller board image:** `SPI_CONTROLLER_MODE = 1`, `SPI_PERIPHERAL_MODE = 0`
  - **Peripheral board image:** `SPI_CONTROLLER_MODE = 0`, `SPI_PERIPHERAL_MODE = 1`

## Logger Interface Configuration

SPI sample logs are expected on **UART0 logger**.

Ensure logger interface is UART0 during build:
- `CONFIG_LOGGER_IF_UART_0=y`

**Important:** Enable `logger_if_uart0` when building SPI sample app.

(It is already enabled in `sr110_rdk_cm55_spi_sample_app_single_board_defconfig`, but verify if you changed config.)

## Wiring

### SPI wiring (single-board loopback)
Connect:
- `G24 -> G9`
- `G22 -> G6`
- `G23 -> G7`
- `G21 -> G8`

### SPI wiring (double-board mode)
Connect Controller board to Peripheral board:
- Controller `G22` -> Peripheral `G6`   (CLK)
- Controller `G21` -> Peripheral `G8`   (CS)
- Controller `G24` -> Peripheral `G7`   (MISO)
- Controller `G23` -> Peripheral `G9`   (MOSI)
- Connect GND between both boards

### UART0 logging wiring (UART bridge)
Connect UART bridge:
- `UART TX -> G45`
- `UART RX -> G44`
- `GND -> any board GND`

## Building and Flashing the Example using VS Code

Use the VS Code flow described in the respective soc vscode guides and the VS Code Extension guide:
- [SR110 Build and Flash with VS Code](../../../docs/SR110/SR110_Build_and_Flash_with_VSCode.md)
- [Astra MCU SDK VS Code Extension User Guide](../../../docs/Astra_MCU_SDK_VSCode_Extension_User_Guide.md)

**Build (VS Code):**
1. Open **Build and Deploy** -> **Build Configurations**.
2. Select **spi_sample_app** in the **Application** dropdown.
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
   cd <sdk-root>/examples/driver_examples/spi_sample_app
   export SRSDK_DIR=<sdk-root>
   make <app_defconfig> BUILD=SRSDK
   ```
2. For faster rebuilds when only app code changes, reuse the app-local installed SDK package:
   ```bash
   cd <sdk-root>/examples/driver_examples/spi_sample_app
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
2. For logging output, click **SERIAL MONITOR** and connect to the **UART0 bridge** port.
   - Use UART bridge adapter connected to `G45` (TX) and `G44` (RX)
   - The logger port is not guaranteed to be consistent across OSes. As a starting point:
     - **Windows:** try the lower-numbered COM port first.
     - **Linux/macOS:** try the higher-numbered port first.
   - If you do not see logs after a reset, switch to the other port.
3. SPI sample logs appear in the logger window, including transfer progress and validation results.

**Expected Logs**

```
SR100.Logger	info	1771926848.982818	SYS 	0	M55	00:00:00:003:917	Application drivers initialization complete without errors.

SR100.Logger	info	1771926848.982818	SYS 	0	M55	00:00:00:008:111	------------------------------------------

SR100.Logger	info	1771926848.982818	SYS 	0	M55	00:00:00:011:477	            Hello  ASTRA                  

SR100.Logger	info	1771926848.982818	SYS 	0	M55	00:00:00:014:844	------------------------------------------

SR100.Logger	info	1771926848.982818	SYS 	0	M55	00:00:00:018:206	System initialization done

SR100.Logger	info	1771926848.982818	SYS 	0	M55	00:00:00:020:807	sr110 SDK version 1.3.0

SR100.Logger	info	1771926848.982818	HAPI	0	M55	00:00:00:023:281	SPI PERIPHERAL INIT FOR HOST API

SR100.Logger	info	1771926848.982818	HAPI	0	M55	00:00:00:026:189	SPI Peripheral Initialized Successfully for Host API.

SR100.Logger	debug	1771926848.982818	HAPI	0	M55	00:00:00:030:080	------------------------------------------

SR100.Logger	debug	1771926848.982818	HAPI	0	M55	00:00:00:033:443	       Host API Router task               

SR100.Logger	debug	1771926848.982818	HAPI	0	M55	00:00:00:036:810	------------------------------------------

SR100.Logger	info	1771926848.982818	HAPI	0	M55	00:00:00:040:163	Active interface is USB

SR100.Logger	info	1771926848.998146	SPI 	0	M55	00:00:00:051:582	Controller Transfer Status = 4

SR100.Logger	info	1771926848.998146	SPI 	0	M55	00:00:00:051:609	Peripheral Transfer Status = 4

SR100.Logger	info	1771926848.998146	SPI 	0	M55	00:00:00:052:282	CONTROLLER RX

SR100.Logger	info	1771926848.998146	SPI 	0	M55	00:00:00:052:305	Controller Match Count: 256 / 256

SR100.Logger	info	1771926848.998146	SPI 	0	M55	00:00:00:052:331	Expected match count does match

SR100.Logger	info	1771926848.998146	SPI 	0	M55	00:00:00:052:355	PERIPHERAL RX

SR100.Logger	info	1771926848.998146	SPI 	0	M55	00:00:00:052:378	Peripheral Match Count: 256 / 256

SR100.Logger	info	1771926848.998146	SPI 	0	M55	00:00:00:052:404	Expected match count does match

SR100.Logger	debug	1771926848.998146	SPI 	0	M55	00:00:00:052:428	SPI Sample App Completed
```
