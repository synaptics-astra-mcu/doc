# DMA Driver Sample Application

## Description

The DMA Driver sample application shows how SR110 moves data in memory without heavy CPU involvement. Instead of copying large buffers in software, it uses the DMA engine and then verifies that the copied data is correct.

The sample includes multiple transfer demonstrations:
- **1D transfer:** copy a large linear buffer from source to destination.
- **2D transfer:** copy matrix-style data (`ROWS x COLS`) with row/stride settings.
- **Linked transfers:** execute a descriptor chain (1D -> 2D -> 1D) in sequence.
- **Software-triggered transfer:** trigger each DMA step manually from software.

During each run, the app logs channel allocation, transfer start/pause/resume, completion status, and validation results. This makes it easy for end users to confirm that DMA setup and data integrity checks are working as expected.

## Prerequisites
- Choose **one** setup path:
  - **CLI**: [Setup and Install SDK using CLI](../../../../docs/Astra_MCU_SDK_Setup_and_Install_CLI.md)
  - **VS Code**: [Setup and Install SDK using VS Code](../../../../docs/Astra_MCU_SDK_Setup_and_Install_VsCode.md)

## Test Mode Configuration

Before building, select the DMA test mode in `dma_sample_app.h`.

Use one of these valid configurations:

1. **Run all tests (default):**
   - `DMA_TRANSFER_ALL=1`
   - `DMA_TRANSFER_1D_COPY=0`
   - `DMA_TRANSFER_2D_COPY=0`
   - `DMA_TRANSFER_LINKED=0`
   - `DMA_TRANSFER_SW_TRIG=0`

2. **Run specific tests:**
   - `DMA_TRANSFER_ALL=0`
   - Configure the required test(s) to `1` (keep the rest `0`):
     - `DMA_TRANSFER_1D_COPY`
     - `DMA_TRANSFER_2D_COPY`
     - `DMA_TRANSFER_LINKED`
     - `DMA_TRANSFER_SW_TRIG`

## Building and Flashing the Example using VS Code

Use the VS Code flow described in the SR110 guide and the VS Code Extension guide:
- [SR110 Build and Flash with VS Code](../../../../docs/SR110/SR110_Build_and_Flash_with_VSCode.md)
- [Astra MCU SDK VS Code Extension User Guide](../../../../docs/Astra_MCU_SDK_VSCode_Extension_User_Guide.md)

**Build (VS Code):**
1. Open **Build and Deploy** -> **Build Configurations**.
2. Select **dma_sample_app** in the **Application** dropdown.
3. Build with **Build (SDK + App)** for the first build, or **Build App** for rebuilds.

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
   make cm55_dma_sample_app_defconfig BOARD=SR110_RDK BUILD=SRSDK
   ```

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

## Running the Application using VS Code Extension

1. Press **RESET** on the board after flashing.
2. For logging output, click **SERIAL MONITOR** and connect to the **DAP logger** port on J14.
   - To make it easier to identify, ensure **only J14** is plugged in (not J13).
   - The logger port is not guaranteed to be consistent across OSes. As a starting point:
     - **Windows:** try the lower-numbered J14 COM port first.
     - **Linux/macOS:** try the higher-numbered J14 port first.
   - If you do not see logs after a reset, switch to the other J14 port.
3. DMA sample logs appear in the logger window, including transfer progress and validation results.

**Expected Logs**

```
SR100.Logger	warning	1769080093.215686	LOGR	0	M55	00:06:31:469:271	Changing logger interface to LOGGER_IF_UART_1
SR100.Logger	info	1769080093.215686	SYS 	0	M55	00:00:00:000:023	System initialization complete without errors.
SR100.Logger	info	1769080093.215686	SYS 	0	M55	00:00:00:003:583	sr110 SDK version 1.3.0
SR100.Logger	info	1769080093.215686	DMA 	0	M55	00:00:00:006:056	Starting DMA Sample Application...
SR100.Logger	info	1769080093.215686	DMA 	0	M55	00:00:00:009:047	Running All DMA Transfer Tests...
SR100.Logger	info	1769080093.215686	DMA 	0	M55	00:00:00:012:436	Channel 0 allocated successfully.
SR100.Logger	info	1769080093.231331	DMA 	0	M55	00:00:00:015:369	[DMA] Preparing 1D transfer: 65536 elements from 0x33f61140 to 0x33f21140
SR100.Logger	info	1769080093.231331	DMA 	0	M55	00:00:00:020:510	Transfer Paused.
SR100.Logger	info	1769080093.247018	DMA 	0	M55	00:00:00:022:633	Transfer resumed.
SR100.Logger	info	1769080093.247018	DMA 	0	M55	00:00:00:026:665	[DMA] 1D transfer successful: 65536 elements verified
SR100.Logger	info	1769080093.247018	DMA 	0	M55	00:00:00:030:562	Channel 0 allocated successfully.
SR100.Logger	info	1769080093.247018	DMA 	0	M55	00:00:00:033:493	Transfer Paused.
SR100.Logger	info	1769080093.262636	DMA 	0	M55	00:00:00:035:617	Transfer resumed.
SR100.Logger	info	1769080093.262636	DMA 	0	M55	00:00:00:037:789	[DMA] 2D transfer successful: 8x8 elements verified
SR100.Logger	info	1769080093.262636	DMA 	0	M55	00:00:00:043:093	Transfer Paused.
SR100.Logger	info	1769080093.262636	DMA 	0	M55	00:00:00:045:216	Transfer resumed.
SR100.Logger	info	1769080093.262636	DMA 	0	M55	00:00:00:051:115	Linked transfers verified successfully.
SR100.Logger	info	1769080093.262636	DMA 	0	M55	00:00:00:055:082	Channel 0 allocated successfully.
SR100.Logger	info	1769080093.278704	DMA 	0	M55	00:00:00:070:316	Final status = 0x10000
SR100.Logger	info	1769080093.278704	DMA 	0	M55	00:00:00:072:726	Transfer completed. Verifying data...
SR100.Logger	info	1769080093.30992	DMA 	0	M55	00:00:00:077:705	SW-triggered DMA transfer successful!
SR100.Logger	info	1769080093.310964	DMA 	0	M55	00:00:00:080:833	All DMA transfers successful.
SR100.Logger	info	1769080093.31193	DMA 	0	M55	00:00:00:083:578	DMA Sample Application Completed.
```