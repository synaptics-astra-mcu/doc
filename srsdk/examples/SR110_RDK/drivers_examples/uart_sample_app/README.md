# UART Driver Sample Application

## Description

The UART Driver sample application validates UART data transmission/reception on UART0, with logs on UART1.

For the standard UART sample path (`uart_sample_app_main`), it demonstrates:
- UART initialization and runtime configuration.
- TX in blocking and non-blocking modes.
- RX in blocking and non-blocking modes.
- Byte-count and UART status verification after each transfer.
- Test baudrate configuration at `230400`.

The sample logs pass/fail status so you can verify UART data path and driver behavior.

Note: the run flow below corresponds to the non-flow-control path (`FLOW_CONTROL_ENABLED=0` in `uart_sample_app.c`).

## Hardware Requirements
- Astra Machina Micro Kit (SR110)
- 2x UART bridge adapters (required):
  - One bridge for UART0 sample TX/RX data path.
  - One bridge for UART1 logger output.

## Prerequisites
- Choose **one** setup path:
  - **CLI**: [Setup and Install SDK using CLI](../../../../docs/Astra_MCU_SDK_Setup_and_Install_CLI.md)
  - **VS Code**: [Setup and Install SDK using VS Code](../../../../docs/Astra_MCU_SDK_Setup_and_Install_VsCode.md)

## Logger Interface Configuration (Important)

This sample must be built with logger output on **UART1**.

- Ensure logger interface is set to UART1 before build (for example, `CONFIG_LOGGER_IF_UART_1=y` or `CONFIG_LOGGER_IF_UART_1_CONSOLE=y`).
- If your tree uses a `_CONSOLE` variant, select the UART1 console option in menuconfig.

**Important:** Build the image with logger interface set to UART1.

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

Use the VS Code flow described in the SR110 guide and the VS Code Extension guide:
- [SR110 Build and Flash with VS Code](../../../../docs/SR110/SR110_Build_and_Flash_with_VSCode.md)
- [Astra MCU SDK VS Code Extension User Guide](../../../../docs/Astra_MCU_SDK_VSCode_Extension_User_Guide.md)

**Build (VS Code):**
1. Ensure logger interface is UART1.
2. Open **Build and Deploy** -> **Build Configurations**.
3. Select **uart_sample_app** in the **Application** dropdown.
4. Build with **Build (SDK + App)** for the first build, or **Build App** for rebuilds.

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
   make cm55_uart_sample_app_defconfig BOARD=SR110_RDK BUILD=SRSDK
   ```
2. Update logger interface to UART1 (if not already set):
   ```bash
   make cm55_uart_sample_app_defconfig BOARD=SR110_RDK BUILD=SRSDK EDIT=1
   ```
   In menuconfig, select UART1 as logger interface and save.

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

## Running the Application

1. Connect UART0 to UART bridge #1 for sample TX/RX.
2. Connect UART1 to UART bridge #2 for logger output.
3. Build and flash the UART sample image.
4. Press **RESET** on the board to start the app.
5. Watch UART1 logger output and wait until:
   - `UART blocking receive: start`

### Host-side RX stimulus using `uart_rx_test.py`

The sample executes RX twice:
- First RX path: blocking receive (`uart_sample_rx_blocking`)
- Second RX path: non-blocking receive (`uart_sample_rx_nonblocking`)

`uart_rx_test.py` sends one full UART payload per run.  
Run it **twice** so both RX stages receive data.

1. Edit script port first:
   - File: `examples/SR110_RDK/drivers_examples/uart_sample_app/uart_rx_test.py`
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
