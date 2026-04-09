# XSPI Driver Sample Application

## Description

The XSPI Driver sample application demonstrates external flash communication and data operations on the supported boards for this application. It performs comprehensive XSPI testing including flash initialization, device identification, and data integrity validation to ensure reliable external storage access.

The sample includes multiple XSPI operations:
- **Flash initialization:** Initialize XSPI interface and bring up external flash communication.
- **Device identification:** Read JEDEC ID to confirm flash device detection and identification.
- **Erase operations:** Erase test regions in flash memory for write operations.
- **Write operations:** Write known data patterns to flash memory using different SPI modes.
- **Read operations:** Read data back from flash memory for verification.
- **Data integrity validation:** Perform byte-by-byte comparison to ensure data correctness.
- **Throughput testing:** Measure transfer performance in 1-bit SDR and 4-bit SDR modes.

During each run, the app logs initialization status, device information, operation progress, throughput measurements, and validation results. This makes it easy for end users to confirm that XSPI setup and flash operations are working as expected.

The latest example structure uses a **common application source tree** with board-specific hardware setup kept under `hw/<BOARD>/`. For this app:
- Common application sources such as `main.c`, `xspi_sample_app.c`, and `xspi_sample_app.h` stay in the app root.
- Application defconfigs are stored under `configs/`.
- Board and hardware-specific setup is selected from `hw/<BOARD>/`, for example `hw/SR110_RDK/`.

The application can also be exported and built as a **standalone app repository**. In that flow, keep this app in its own directory, point `SRSDK_DIR` to the SDK root, and build from the app directory itself. For the full application workflow model, see [Astra MCU SDK User Guide](../../../docs/Astra_MCU_SDK_User_Guide.md).

## Supported Boards

This application supports:
- `SR110_RDK`
- `SL2610_RDK`

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
- `sl2610_rdk_cm52_xspi_sample_app_defconfig`
- `sr110_rdk_cm55_xspi_sample_app_defconfig`


For this app, the default defconfig is:
   - SR110_RDK: `sr110_rdk_cm55_xspi_sample_app_defconfig`
   - SL2610_RDK: `sl2610_rdk_cm52_xspi_sample_app_defconfig`

## Building and Flashing the Example using VS Code

Use the VS Code flow described in the respective soc vscode guides and the VS Code Extension guide:
- [SR110 Build and Flash with VS Code](../../../docs/SR110/SR110_Build_and_Flash_with_VSCode.md)
- [SL2610 Build and Flash with VS Code](../../../docs/SL2610/SL2610_Build_and_Flash_with_VSCode.md)
- [Astra MCU SDK VS Code Extension User Guide](../../../docs/Astra_MCU_SDK_VSCode_Extension_User_Guide.md)

**Build (VS Code):**
1. Open **Build and Deploy** -> **Build Configurations**.
2. Select **xspi_sample_app** in the **Application** dropdown.
3. Select the required defconfig for the application.
4. Build with **Build (SDK + App)** for the first build, or **Build App** for rebuilds.

**Flash (VS Code):**
1. Use **Image Conversion** to generate the flash image.
2. Use **Image Flashing** (SWD/JTAG) to flash the firmware image.

---

## Building and Flashing the Example using CLI

Use the CLI flow described in the respective build guide:
- [SR110 Build and Flash with CLI](../../../docs/SR110/SR110_Build_and_Flash_with_CLI.md)
- [SL2610 Build and Flash with CLI](../../../docs/SL2610/SL2610_Build_and_Flash_with_CLI.md)
- [Astra MCU SDK User Guide](../../../docs/Astra_MCU_SDK_User_Guide.md)

**Build (CLI):**
1. Build from the application directory itself:
   ```bash
   cd <sdk-root>/examples/driver_examples/xspi_sample_app
   export SRSDK_DIR=<sdk-root>
   make <app_defconfig> BUILD=SRSDK
   ```
2. For faster rebuilds when only app code changes, reuse the app-local installed SDK package:
   ```bash
   cd <sdk-root>/examples/driver_examples/xspi_sample_app
   export SRSDK_DIR=<sdk-root>
   make build
   ```
3. If this app has been exported to its own repository, use the same commands from that exported app directory after setting `SRSDK_DIR` to the SDK root.

**Build outputs (CLI):**
- Application binary: `<app-dir>/out/<target>/release/<target>.elf`
- App-local SDK package: `<app-dir>/install/<BOARD>/<BUILD_TYPE>/`

**Flash (CLI):**

**Flash SR110**

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
     -m55_image "<sdk-root>/examples/driver_examples/xspi_sample_app/out/sr110_cm55_fw/release/sr110_cm55_fw.elf" \
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

**Flash SL2610**

1. Build the SL2610 bootloader image.
   ```
   cd <sdk-root>
   export SRSDK_DIR=<sdk-root>
   make <SL2610_Bootloader_defconfig> BOARD=<BOARD>
   make astrasdk
   ```

2. Generate the system sub-image.
   ```
   cd <sdk-root>/examples/driver_examples/xspi_sample_app
   export SRSDK_DIR=<sdk-root>
   make imagegen
   ```

3. Flash/download image to target.

   Refer: [SL2610 Platform Guide](../../../docs/SL2610/SL2610_Build_and_Flash_with_CLI.md)

---

## Running the Application using VS Code Extension

1. Press **RESET** on the board after flashing.
2. For logging output, click **SERIAL MONITOR** and connect to the **DAP logger** port on J14.
   - To make it easier to identify, ensure **only J14** is plugged in (not J13).
   - The logger port is not guaranteed to be consistent across OSes. As a starting point:
     - **Windows:** try the lower-numbered J14 COM port first.
     - **Linux/macOS:** try the higher-numbered J14 port first.
   - If you do not see logs after a reset, switch to the other J14 port.
3. XSPI sample logs appear in the logger window, including JEDEC ID, throughput timing, and data verification results.

**Expected Logs**

```
0391470066:[0][WRN][LOGR]:Changing logger interface to LOGGER_IF_UART_1
0000000023:[0][INF][SYS ]:Application drivers initialization complete without errors.
0000004210:[0][INF][SYS ]:------------------------------------------
0000007576:[0][INF][SYS ]:            Hello  ASTRA
0000010943:[0][INF][SYS ]:------------------------------------------
0000014309:[0][INF][SYS ]:System initialization done
0000016910:[0][INF][SYS ]:sr110 SDK version 1.3.0
0000019385:[0][INF][FLSH]:Starting XSPI Sample Application...
0000022438:[0][INF][FLSH]:Starting Flash Initialization Test...
0000025569:[0][INF][FLSH]:[FLASH] Init successful
0000028029:[0][INF][FLSH]:Flash Initialization Test Passed.
0000030969:[0][INF][FLSH]:Starting Read Device ID Test...
0000033822:[0][INF][FLSH]:[FLASH] JEDEC ID: 0x1860C8 (MFG=0xC8)
0000036955:[0][INF][FLSH]:Read Device ID Test Passed.
0000039607:[0][INF][FLSH]:Starting Read/Write Throughput Test...
0000042786:[0][INF][FLSH]:Flash RW throughput test started
0000045678:[0][INF][FLSH]:Testing throughput: IO=1-bit, Mode=SDR
0000048858:[0][INF][FLSH]:[FLASH] RW throughput test (IO=1-bit, Mode=SDR)
0000052480:[0][INF][FLSH]:Configuring XSPI instance 0 for DIRECT mode with IO=1, DTR=0
0000056735:[0][INF][FLSH]:flash_erase: addr=0x0, len=0x200
0000059628:[0][INF][XSPI]:Erase 4k @ 0x0
0000090088:[0][INF][FLSH]:[FLASH] Write: 512 bytes in 0 ms
0000093318:[0][INF][FLSH]:[FLASH] Read : 512 bytes in 0 ms
0000096222:[0][INF][FLSH]:[FLASH] RW throughput test passed
0000099164:[0][INF][FLSH]:Throughput test PASSED: IO=1-bit, Mode=SDR
0000102536:[0][INF][FLSH]:Testing throughput: IO=4-bit, Mode=SDR
0000105716:[0][INF][FLSH]:[FLASH] RW throughput test (IO=4-bit, Mode=SDR)
0000109339:[0][INF][FLSH]:Configuring XSPI instance 0 for DIRECT mode with IO=4, DTR=0
0000113593:[0][INF][FLSH]:flash_erase: addr=0x0, len=0x200
0000116469:[0][INF][XSPI]:Erase 4k @ 0x0
0000145651:[0][INF][FLSH]:[FLASH] Write: 512 bytes in 1 ms
0000148742:[0][INF][FLSH]:[FLASH] Read : 512 bytes in 0 ms
0000151647:[0][INF][FLSH]:[FLASH] RW throughput test passed
0000154588:[0][INF][FLSH]:Throughput test PASSED: IO=4-bit, Mode=SDR
0000157960:[0][INF][FLSH]:Flash RW throughput test completed, final status=0
0000161715:[0][INF][FLSH]:Read/Write Throughput Test Passed.
0000164702:[0][INF][FLSH]:All Flash Tests Completed Successfully.
0000167954:[0][INF][FLSH]:XSPI Sample Application Completed Successfully.
```
