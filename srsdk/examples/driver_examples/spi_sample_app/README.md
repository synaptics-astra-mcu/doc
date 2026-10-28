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
- `SL2610_RDK`

Select the defconfig that matches your target board, and the build system will pick the corresponding board-specific hardware setup from `hw/<BOARD>/`.

## Hardware Requirements
- Astra Machina Micro Kit (SR110) or SL2610 RDK/PEK board
- UART bridge adapter (required for logging)
- SPI jumper wires
- For double-board mode: 2x Astra Machina Micro boards

## Prerequisites
- Choose **one** setup path:
  - **CLI**: [Setup and Install SDK using CLI](../../../docs/Astra_MCU_SDK_Setup_and_Install_CLI.md)
  - **VS Code**: [Setup and Install SDK using VS Code](../../../docs/Astra_MCU_SDK_Setup_and_Install_VsCode.md)

## Project Configuration Selection

Before building, choose the project configuration (defconfig) that matches your target board.

You can:
- Select the required defconfig directly from the application's `configs/` directory.
- Run `make list_defconfigs` from the application directory to list all supported defconfigs.

**Available defconfigs:**
- `sr110_rdk_cm55_spi_sample_app_double_board_controller_defconfig`
- `sr110_rdk_cm55_spi_sample_app_double_board_peripheral_defconfig`
- `sr110_rdk_cm55_spi_sample_app_preroll_controller_defconfig`
- `sr110_rdk_cm55_spi_sample_app_single_board_defconfig`
- `sl2610_rdk_cm52_spi_sample_app_controller_only_defconfig`
- `sl2610_rdk_cm52_spi_sample_app_controller_only_dma_defconfig`

For this app, the default defconfig is:
   - `sr110_rdk_cm55_spi_sample_app_single_board_defconfig`
   - `sl2610_rdk_cm52_spi_sample_app_controller_only_defconfig`

## Logger Interface Configuration

SPI sample logs are expected on **UART0 logger**.

Ensure logger interface is UART0 during build:
- `CONFIG_LOGGER_IF_UART_0=y`

**Important:** Enable `logger_if_uart0` when building SPI sample app.

(It is already enabled in `sr110_rdk_cm55_spi_sample_app_single_board_defconfig`, but verify if you changed config.)

For SL2610 boards, use:
- `sl2610_rdk_cm52_spi_sample_app_controller_only_defconfig` for controller-only bring-up
- `sl2610_rdk_cm52_spi_sample_app_controller_only_dma_defconfig` for DMA controller bring-up

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

**Note (SL2610_RDK):**
For controller-only and controller-only DMA test, SPI signals can be measured with a logic analyzer (LA) on CLK, CS, MOSI, and MISO pins to verify waveform timing and data activity.

## Building and Flashing the Example using VS Code

Use the VS Code flow described in the respective soc vscode guides and the VS Code Extension guide:
- [SR110 Build and Flash with VS Code](../../../docs/SR110/SR110_Build_and_Flash_with_VSCode.md)
- [SL2610 Build and Flash with VS Code](../../../docs/SL2610/SL2610_Build_and_Flash_with_VSCode.md)
- [Astra MCU SDK VS Code Extension User Guide](../../../docs/Astra_MCU_SDK_VSCode_Extension_User_Guide.md)

**Build (VS Code):**
1. Open **Build and Deploy** -> **Build Configurations**.
2. Select the **spi_sample_app** project configuration in the **Project Configuration** dropdown.
3. Build with **Build (SDK+Project)** for the first build, or **Build (Project)** for rebuilds.

**Flash (VS Code):**
1. For `SR110`:
   - Use **Image Conversion** to generate the flash image.
   - Use **Image Flashing** with **SWD/JTAG** to flash the firmware image.
2. For `SL2610`:
   - Use the SL2610 image-generation flow to generate the required sub-image.
   - Open **Image Flashing (SL2610)**.
   - Select **Flash Target** as **M52 Image**.
   - In **Image Path**, browse to and select the generated sub-image file, such as `sysmgr.subimg.gz`.
   - Start the flashing operation to program the image to the target.

---

## Building and Flashing the Example using CLI

Use the CLI flow described in the respective build guide:
- [SR110 Build and Flash with CLI](../../../docs/SR110/SR110_Build_and_Flash_with_CLI.md)
- [SL2610 Build and Flash with CLI](../../../docs/SL2610/SL2610_Build_and_Flash_with_CLI.md)
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

**Flash SR110 (CLI):**
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
     -m55_image "<sdk-root>/examples/driver_examples/spi_sample_app/out/sr110_cm55_fw/release/sr110_cm55_fw.elf" \
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

**Flash SL2610 (CLI):**

1. Build the SL2610 bootloader image.
   ```bash
   cd <sdk-root>
   export SRSDK_DIR=<sdk-root>
   make <SL2610_Bootloader_defconfig> BOARD=<BOARD>
   make astrasdk
   ```

2. Generate the system sub-image.
   ```bash
   cd <sdk-root>/examples/driver_examples/spi_sample_app
   export SRSDK_DIR=<sdk-root>
   make imagegen
   ```

3. Flash/download image to target.

   Refer: [SL2610 Platform Guide](../../../docs/SL2610/SL2610_Build_and_Flash_with_CLI.md)

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

**SR110 (Single-board):**

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

**SL2610 (Controller-only, non-DMA):**

```
BL: MCU Init...
send command successfully
SW_STRAP:0x02
Init DDR4 8x2 @ 3200
DHL:v0p41 PT:v0p40 ID:0x21010000

Boot from USB
USB MOUNTED
0000000000:[0][INF][SYS ]:Application drivers initialization complete without errors.
0000000000:[0][INF][SYS ]:sl2610 SDK version 1.5.0
0000000000:[0][INF][USB ]:USB device unmounted
0000000004:[0][INF][SPI ]:CONTROLLER INIT
0000000008:[0][INF][SPI ]:Time for controller_transfer = 0 msec
0000000016:[0][INF][SPI ]:Controller Transfer Status = 4
0000000017:[0][INF][SPI ]:CONTROLLER RX
0000000017:[0][INF][SPI ]:Controller Match Count: 128 / 256
****************
0000000017:[0][ERR][SPI ]:Expected match count does not match
****************
0000000017:[0][DBG][SPI ]:SPI Sample App Completed
```

**SL2610 (Controller-only DMA):**

```
BL: MCU Init...
send command successfully
SW_STRAP:0x02
Init DDR4 8x2 @ 3200
DHL:v0p41 PT:v0p40 ID:0x21010000

Boot from USB
USB MOUNTED
0000000000:[0][INF][SYS ]:Application drivers initialization complete without errors.
0000000000:[0][INF][SYS ]:sl2610 SDK version 1.5.0
0000000000:[0][INF][USB ]:USB device unmounted
0000000004:[0][INF][SPI ]:CONTROLLER INIT
0000000008:[0][INF][SPI ]:Time for controller_transfer = 0 msec
0000000016:[0][INF][SPI ]:SPI Instance = 0, TX status = 10000, remaining = 0, channel = 16
0000000016:[0][INF][SPI ]:SPI Instance = 0, RX status = 10000,                          remaining = 0, channel = 6
0000000016:[0][INF][SPI ]:CONTROLLER RX
0000000016:[0][INF][SPI ]:Controller Match Count: 128 / 256
****************
0000000016:[0][ERR][SPI ]:Expected match count does not match
****************
0000000016:[0][DBG][SPI ]:SPI Sample App Completed
```
