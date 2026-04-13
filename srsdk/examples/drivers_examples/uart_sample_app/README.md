# UART Driver Sample Application

## Description

The UART Driver sample application demonstrates UART communication and data transfer operations on the supported boards for this application. It performs comprehensive UART testing including initialization, transmission, reception, and data integrity validation to ensure reliable serial communication.

The sample includes multiple UART operations:
- **UART initialization:** Initialize UART0 for data transfer and UART1 for logging output.
- **Blocking transmission:** Send data using blocking write operations with status verification.
- **Non-blocking transmission:** Send data using non-blocking write operations with callback handling.
- **Blocking reception:** Receive data using blocking read operations with byte-count verification.
- **Non-blocking reception:** Receive data using non-blocking read operations with interrupt handling.
- **Data integrity validation:** Compare transmitted and received data to ensure transfer correctness.

During each run, the app logs initialization status, transfer progress, byte counts, and validation results. This makes it easy for end users to confirm that UART setup and data operations are working as expected.

The latest example structure uses a **common application source tree** with board-specific hardware setup kept under `hw/<BOARD>/`. For this app:
- Common application sources such as `main.c`, `uart_sample_app.c`, and `uart_sample_app.h` stay in the app root.
- Application defconfigs are stored under `configs/`.
- Board and hardware-specific setup is selected from `hw/<BOARD>/`, for example `hw/SR110_RDK/`.

The application can also be exported and built as a **standalone app repository**. In that flow, keep this app in its own directory, point `SRSDK_DIR` to the SDK root, and build from the app directory itself. For the full application workflow model, see [Astra MCU SDK User Guide](../../../docs/Astra_MCU_SDK_User_Guide.md).

## Supported Boards

This application supports:
- `SR110_RDK`

Select the defconfig that matches your target board, and the build system will pick the corresponding board-specific hardware setup from `hw/<BOARD>/`.

## Hardware Requirements
- Astra Machina Micro Kit (SR110)
- 2x UART bridge adapters (required):
  - One bridge for UART0 sample TX/RX data path.
  - One bridge for UART1 logger output.

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
- `sr110_rdk_cm55_uart_sample_app_flow_control_client_defconfig`
- `sr110_rdk_cm55_uart_sample_app_flow_control_host_defconfig`
- `sr110_rdk_cm55_uart_sample_app_tx_rx_defconfig`

For this app, the default defconfig is:
   - `sr110_rdk_cm55_uart_sample_app_tx_rx_defconfig`

## UART Connections (UART Bridge)

### UART0 for sample TX/RX
- `UART TX -> G45`
- `UART RX -> G44`
- `GND -> any board GND`

Connect this bridge/device to **UART0** for `uart_rx_test.py` data path.

### UART1 for logger output
Use UART1 as logger interface and open a separate logger terminal on that port.

Two UART bridges are required: one connected to UART0 and one connected to UART1.

## Building and Flashing the Example using VS Code

Use the VS Code flow described in the respective soc vscode guides and the VS Code Extension guide:
- [SR110 Build and Flash with VS Code](../../../docs/SR110/SR110_Build_and_Flash_with_VSCode.md)
- [Astra MCU SDK VS Code Extension User Guide](../../../docs/Astra_MCU_SDK_VSCode_Extension_User_Guide.md)

**Build (VS Code):**
1. Open **Build and Deploy** -> **Build Configurations**.
2. Select **uart_sample_app** in the **Application** dropdown.
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
   cd <sdk-root>/examples/driver_examples/uart_sample_app
   export SRSDK_DIR=<sdk-root>
   make <app_defconfig> BUILD=SRSDK
   ```
2. For faster rebuilds when only app code changes, reuse the app-local installed SDK package:
   ```bash
   cd <sdk-root>/examples/driver_examples/uart_sample_app
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
     -m55_image "<sdk-root>/examples/driver_examples/uart_sample_app/out/sr110_cm55_fw/release/sr110_cm55_fw.elf" \
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
   - Use UART bridge adapter connected to UART1 for logger output
   - The logger port is not guaranteed to be consistent across OSes. As a starting point:
     - **Windows:** try the lower-numbered COM port first.
     - **Linux/macOS:** try the higher-numbered port first.
   - If you do not see logs after a reset, switch to the other port.
3. UART sample logs appear in the logger window, including transfer progress and validation results.
4. For data transfer testing, connect a separate UART bridge to UART0 and use the provided test script.

### Host-side RX stimulus using `uart_rx_test.py`

The sample executes RX twice:
- First RX path: blocking receive (`uart_sample_rx_blocking`)
- Second RX path: non-blocking receive (`uart_sample_rx_nonblocking`)

`uart_rx_test.py` sends one full UART payload per run. Run it **twice** so both RX stages receive data.

1. Edit script port first:
   - File: `uart_rx_test.py` in the uart_sample_app directory
   - Update `SERIAL_PORT` to your **UART0** bridge COM/device (for example `COM7` or `/dev/ttyUSB0`).
2. After UART1 log shows `UART blocking receive: start`, run:
   ```bash
   python uart_rx_test.py
   ```
3. Run the same command a second time:
   ```bash
   python uart_rx_test.py
   ```

**Expected Logs**

```
SR100.Logger	warning	1771918316.391563	LOGR	0	M55	00:06:31:469:221	Changing logger interface to LOGGER_IF_UART_1_CONSOLE

SR100.Logger	info	1771918316.391563	SYS 	0	M55	00:00:00:000:023	Application drivers initialization complete without errors.

SR100.Logger	info	1771918316.423233	SYS 	0	M55	00:00:00:004:209	sr110 SDK version 1.3.0

SR100.Logger	info	1771918326.424638	UART	0	M55	00:00:10:007:094	Starting UART Sample App! 

SR100.Logger	debug	1771918326.424638	UART	0	M55	00:00:10:007:123	Baudrate set: 230400

SR100.Logger	debug	1771918326.424638	UART	0	M55	00:00:10:007:153	Achieved Baudrate: 230414

SR100.Logger	info	1771918326.431161	UART	0	M55	00:00:10:007:177	UART blocking send: start

SR100.Logger	info	1771918326.525943	SYS 	0	M55	00:00:10:064:423	TX blocking count: 1000

SR100.Logger	info	1771918326.525943	UART	0	M55	00:00:10:064:453	TX BLOCKING SUCCESSFULL

SR100.Logger	info	1771918326.525943	UART	0	M55	00:00:10:064:476	UART blocking send: end

SR100.Logger	debug	1771918326.54184	UART	0	M55	00:00:10:064:504	Baudrate set: 230400

SR100.Logger	debug	1771918326.54184	UART	0	M55	00:00:10:064:534	Achieved Baudrate: 230414

SR100.Logger	info	1771918326.54184	UART	0	M55	00:00:10:064:558	UART non-blocking send: start

SR100.Logger	debug	1771918326.54184	UART	0	M55	00:00:10:112:267	uart: event 0x0 raised

SR100.Logger	debug	1771918326.557209	UART	0	M55	00:00:10:114:098	uart: event 0x0 raised

SR100.Logger	info	1771918326.557209	SYS 	0	M55	00:00:10:114:123	TX non blocking count: 1000

SR100.Logger	info	1771918326.557209	UART	0	M55	00:00:10:114:153	TX NON BLOCKING SUCCESSFULL

SR100.Logger	info	1771918326.557209	UART	0	M55	00:00:10:114:177	UART non-blocking send: end

SR100.Logger	debug	1771918326.557209	UART	0	M55	00:00:10:114:206	Baudrate set: 230400

SR100.Logger	debug	1771918326.58846	UART	0	M55	00:00:10:114:235	Achieved Baudrate: 230414

SR100.Logger	info	1771918326.58846	UART	0	M55	00:00:10:114:259	UART blocking receive: start

SR100.Logger	info	1771918327.281283	SYS 	0	M55	00:00:10:887:013	RX blocking count: 1000

SR100.Logger	debug	1771918327.344187	SYS 	0	M55	00:00:10:887:037	uart_sample_rx_blocking: Received: UART FLOW CONTROL TESTING: 0123456789 ABCDEFGHIJKLMNOPQRSTUVWXYZ : Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culp

SR100.Logger	info	1771918327.344187	UART	0	M55	00:00:10:887:092	RX BLOCKING SUCCESSFULL

SR100.Logger	info	1771918327.344187	UART	0	M55	00:00:10:887:115	UART blocking receive: end

SR100.Logger	debug	1771918327.344187	UART	0	M55	00:00:10:888:145	Baudrate set: 230400

SR100.Logger	debug	1771918327.359815	UART	0	M55	00:00:10:888:175	Achieved Baudrate: 230414

SR100.Logger	info	1771918327.375445	UART	0	M55	00:00:10:888:199	UART non-blocking receive: start

SR100.Logger	debug	1771918339.946425	UART	0	M55	00:00:23:545:631	uart: event 0x0 raised

SR100.Logger	info	1771918339.946425	SYS 	0	M55	00:00:23:545:656	RX non blocking count: 1000

SR100.Logger	debug	1771918340.007573	SYS 	0	M55	00:00:23:546:681	uart_sample_rx_nonblocking: Received: UART FLOW CONTROL TESTING: 0123456789 ABCDEFGHIJKLMNOPQRSTUVWXYZ : Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in c

SR100.Logger	info	1771918340.007573	UART	0	M55	00:00:23:546:736	RX NON BLOCKING SUCCESSFULL

SR100.Logger	info	1771918340.007573	UART	0	M55	00:00:23:546:760	UART non-blocking receive: end

SR100.Logger	info	1771918340.036705	UART	0	M55	00:00:23:546:784	UART Sample App Completed!

```
