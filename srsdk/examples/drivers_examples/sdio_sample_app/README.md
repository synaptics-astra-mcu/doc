# SDIO Driver Sample Application

## Description

The SDIO Driver sample application demonstrates SDIO host/device communication and data transfer operations on the supported boards for this application. It performs comprehensive SDIO testing including initialization, register access, and data transfer validation to ensure reliable SDIO functionality.

The sample includes multiple SDIO operations:
- **SDIO host initialization:** Initialize SDIO host interface and configure clock/voltage settings.
- **Device initialization:** Complete SDIO device bring-up using CMD0/CMD5/CMD52-based setup and readiness checks.
- **Register access:** Perform CMD52 register read/write operations for device configuration.
- **Data transfer verification:** Execute CMD53 data transfers in both byte mode and block mode.
- **Data integrity validation:** Compare transmitted and received buffers to ensure transfer correctness.

During each run, the app logs initialization status, command execution, transfer progress, and validation results. This makes it easy for end users to confirm that SDIO setup and data operations are working as expected.

The latest example structure uses a **common application source tree** with board-specific hardware setup kept under `hw/<BOARD>/`. For this app:
- Common application sources such as `main.c`, `sdio_sample_app.c`, and `sdio_sample_app.h` stay in the app root.
- Application defconfigs are stored under `configs/`.
- Board and hardware-specific setup is selected from `hw/<BOARD>/`, for example `hw/SR110_RDK/`.

The application can also be exported and built as a **standalone app repository**. In that flow, keep this app in its own directory, point `SRSDK_DIR` to the SDK root, and build from the app directory itself. For the full application workflow model, see [Astra MCU SDK User Guide](../../../docs/Astra_MCU_SDK_User_Guide.md).

## Supported Boards

This application supports:
- `SR110_RDK`

Select the defconfig that matches your target board, and the build system will pick the corresponding board-specific hardware setup from `hw/<BOARD>/`.

## Hardware Requirements
- Astra Machina Micro (SR110)
- **WLAN chip module 4612 or 43711 attached to the board** (required)

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
- `sr110_evb_cm55_sdio_sample_app_defconfig`
- `sr110_rdk_cm55_sdio_sample_app_defconfig`


For this app, the default defconfig is:
   - `sr110_rdk_cm55_sdio_sample_app_defconfig`

## Building and Flashing the Example using VS Code

Use the VS Code flow described in the respective soc vscode guides and the VS Code Extension guide:
- [SR110 Build and Flash with VS Code](../../../docs/SR110/SR110_Build_and_Flash_with_VSCode.md)
- [Astra MCU SDK VS Code Extension User Guide](../../../docs/Astra_MCU_SDK_VSCode_Extension_User_Guide.md)

**Build (VS Code):**
1. Open **Build and Deploy** -> **Build Configurations**.
2. Select **sdio_sample_app** in the **Application** dropdown.
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
   cd <sdk-root>/examples/driver_examples/sdio_sample_app
   export SRSDK_DIR=<sdk-root>
   make <app_defconfig> BUILD=SRSDK
   ```
2. For faster rebuilds when only app code changes, reuse the app-local installed SDK package:
   ```bash
   cd <sdk-root>/examples/driver_examples/sdio_sample_app
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
     -m55_image "<sdk-root>/examples/driver_examples/sdio_sample_app/out/sr110_cm55_fw/release/sr110_cm55_fw.elf" \
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
3. SDIO sample logs appear in the logger window, including CMD52/CMD53 transfer checks and validation results.

**Expected Logs**

```
SR100.Logger	warning	1771669272.006958	LOGR	0	M55	00:06:31:469:192	Changing logger interface to LOGGER_IF_UART_1

SR100.Logger	info	1771669272.006958	SYS 	0	M55	00:00:00:000:023	Application drivers initialization complete without errors.

SR100.Logger	info	1771669272.017984	SYS 	0	M55	00:00:00:004:210	sr110 SDK version 1.3.0

SR100.Logger	info	1771669272.133226	SDIO	0	M55	00:00:00:106:701	SDIO Init Success

SR100.Logger	info	1771669272.133226	SDIO	0	M55	00:00:00:107:219	SDIO Host Init Success

SR100.Logger	info	1771669272.133226	SDIO	0	M55	00:00:00:109:501	Driver Strength Register: 0x00001007

SR100.Logger	info	1771669272.133226	SDIO	0	M55	00:00:00:110:895	SDIO device initialized successfully on instance 1

SR100.Logger	info	1771669272.14888	SDIO	0	M55	00:00:00:110:921	SDIO IO Device Init Success

SR100.Logger	info	1771669272.14888	SDIO	0	M55	00:00:00:110:945	SDIO Initialization Complete

SR100.Logger	info	1771669272.14888	SDIO	0	M55	00:00:00:111:696	Read Chip ID : 0xbf aa 40 10

SR100.Logger	info	1771669272.14888	SDIO	0	M55	00:00:00:111:723	CMD52 Read/Write verification: start

SR100.Logger	info	1771669274.689167	SDIO	0	M55	00:00:02:673:792	CMD52 Verification successful! All values matched

SR100.Logger	info	1771669274.689167	SDIO	0	M55	00:00:02:673:819	CMD52 Read/Write verification: completed

SR100.Logger	info	1771669274.689167	SDIO	0	M55	00:00:02:675:793	Block Write Completed!

SR100.Logger	info	1771669274.717445	SDIO	0	M55	00:00:02:675:816	Setting the buffer to 0

SR100.Logger	info	1771669274.718649	SDIO	0	M55	00:00:02:684:793	Block Read Completed!

SR100.Logger	info	1771669274.718649	SDIO	0	M55	00:00:02:684:820	All values matched!

SR100.Logger	info	1771669274.720658	SDIO	0	M55	00:00:02:685:793	Block Write Completed!

SR100.Logger	info	1771669274.721665	SDIO	0	M55	00:00:02:685:816	Setting the buffer to 0

SR100.Logger	info	1771669274.73682	SDIO	0	M55	00:00:02:694:794	Block Read Completed!

SR100.Logger	info	1771669274.73682	SDIO	0	M55	00:00:02:694:820	All values matched!

SR100.Logger	info	1771669274.73682	SDIO	0	M55	00:00:02:695:799	DATA MATCHED!!

SR100.Logger	info	1771669274.73682	SDIO	0	M55	00:00:02:695:841	Block Write Completed!

SR100.Logger	info	1771669274.73682	SDIO	0	M55	00:00:02:695:864	Setting the buffer to 0

SR100.Logger	info	1771669274.752534	SDIO	0	M55	00:00:02:704:794	Block Read Completed!

SR100.Logger	info	1771669274.752534	SDIO	0	M55	00:00:02:704:817	All values matched!

SR100.Logger	info	1771669274.752534	SDIO	0	M55	00:00:02:704:852	DATA MATCHED!!

SR100.Logger	info	1771669274.752534	SDIO	0	M55	00:00:02:705:794	Block Write Completed!

SR100.Logger	info	1771669274.752534	SDIO	0	M55	00:00:02:705:817	Setting the buffer to 0

SR100.Logger	info	1771669274.752534	SDIO	0	M55	00:00:02:714:795	Block Read Completed!

SR100.Logger	info	1771669274.780773	SDIO	0	M55	00:00:02:714:821	All values matched!

SR100.Logger	info	1771669274.784571	SDIO	0	M55	00:00:02:715:800	DATA MATCHED!!

SR100.Logger	info	1771669274.784571	SDIO	0	M55	00:00:02:715:822	Setting the buffer to 0

SR100.Logger	info	1771669274.784571	SDIO	0	M55	00:00:02:723:862	Block Read Completed!

SR100.Logger	info	1771669274.786466	SDIO	0	M55	00:00:02:723:885	Setting the buffer to 0

SR100.Logger	info	1771669274.786466	SDIO	0	M55	00:00:02:732:797	All values matched!

SR100.Logger	info	1771669274.786466	SDIO	0	M55	00:00:02:732:820	Block Read Completed1!

SR100.Logger	info	1771669274.786466	SDIO	0	M55	00:00:02:732:843	Setting the buffer to 0

SR100.Logger	info	1771669274.786466	SDIO	0	M55	00:00:02:741:796	Block Read Completed!

SR100.Logger	info	1771669274.800496	SDIO	0	M55	00:00:02:742:796	Block Write Completed!

SR100.Logger	info	1771669274.800496	SDIO	0	M55	00:00:02:743:796	Block Write Completed!

SR100.Logger	info	1771669274.800496	SDIO	0	M55	00:00:02:743:819	Setting the buffer to 0

SR100.Logger	info	1771669274.800496	SDIO	0	M55	00:00:02:752:796	Block Read Completed!

SR100.Logger	info	1771669274.800496	SDIO	0	M55	00:00:02:752:846	All values matched!

SR100.Logger	info	1771669274.800496	SDIO	0	M55	00:00:02:753:801	DATA MATCHED!!

SR100.Logger	info	1771669274.800496	SDIO	0	M55	00:00:02:753:824	SDIO Sample App Completed! 


```
