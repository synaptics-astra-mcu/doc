# NAND Driver Sample Application

## Description

The NAND Driver sample application demonstrates external SPI-NAND flash operations and data validation on the supported boards for this application. It performs NAND initialization, device identification, erase/program/read validation, randomizer verification, page and multi-page access tests, and throughput measurement to help confirm reliable NAND functionality.

The sample includes multiple NAND validation flows:
- **Flash initialization:** Bring up the NAND flash interface and verify driver initialization.
- **Device identification:** Read the NAND device ID to confirm flash detection.
- **Basic read/write/erase test:** Erase a test region, write known data, read it back, and validate the result.
- **Page and multi-page read tests:** Exercise page-based APIs, including page + OOB access and multi-page reads.
- **Randomizer test:** Write and read back randomized page data to validate the NAND randomizer path.
- **Throughput test:** Measure NAND read/write throughput using the configured test buffer.

During each run, the app logs initialization status, NAND test progress, validation results, and final pass/fail status. This makes it easy for end users to confirm that NAND setup and flash operations are working as expected.

The latest example structure uses a **common application source tree** with board-specific hardware setup kept under `hw/<BOARD>/`. For this app:
- Common application sources such as `main.c`, `nand_sample_app.c`, and `nand_sample_app.h` stay in the app root.
- Application defconfigs are stored under `configs/`.
- Board and hardware-specific setup is selected from `hw/<BOARD>/`, for example `hw/SL2610_RDK/`.

The application can also be exported and built as a **standalone app repository**. In that flow, keep this app in its own directory, point `SRSDK_DIR` to the SDK root, and build from the app directory itself. For the full application workflow model, see [Astra MCU SDK User Guide](../../../docs/Astra_MCU_SDK_User_Guide.md).

## Supported Boards

This application supports:
- `SL2610_RDK`

Select the defconfig that matches your target board, and the build system will pick the corresponding board-specific hardware setup from `hw/<BOARD>/`.

## Prerequisites
- Choose **one** setup path:
  - **CLI**: [Setup and Install SDK using CLI](../../../docs/Astra_MCU_SDK_Setup_and_Install_CLI.md)
  - **VS Code**: [Setup and Install SDK using VS Code](../../../docs/Astra_MCU_SDK_Setup_and_Install_VsCode.md)

## Project Configuration Selection

Before building, select the defconfig and NAND test mode that match your target setup.

You can:
- Select the required defconfig directly from the application's `configs/` directory.
- Run `make list_defconfigs` from the application directory to list supported defconfigs.

**Available defconfigs:**
- `sl2610_rdk_cm52_nand_sample_app_defconfig`

The NAND test flow is controlled from the application's Kconfig menu. The available top-level test selections are:
- `APP_NAND_BASIC_TEST`
- `APP_NAND_MULTI_REW_TEST`
- `APP_NAND_READ_RANDMIZER_TEST`
- `APP_NAND_THROUGHPUT_TEST`

When `APP_NAND_BASIC_TEST` is selected, the app also enables:
- `APP_NAND_BASIC_REW_TEST`
- `APP_NAND_READ_TEST`

You can adjust related NAND parameters such as test block/page selection, page size, pages per block, and throughput buffer size through the same app configuration flow.

## Logger Interface Configuration

The provided NAND defconfig enables the UART0 logger:
- `CONFIG_LOGGER_IF_UART_0=y`

Observe logs on the UART0 console unless you intentionally change the logger configuration.

## Connections

### UART0 logging wiring (UART bridge)

**REV-A**

Connect UART bridge:
- `UART TX -> G8`
- `UART RX -> G28`
- `GND -> G6`


**REV-B**

Connect UART bridge:
- `UART TX -> G8`
- `UART RX -> G10`
- `GND -> G6`

## Building and Flashing the Example using VS Code

Use the VS Code flow described in the respective platform guide and the VS Code Extension guide:
- [SL2610 Build and Flash with VS Code](../../../docs/SL2610/SL2610_Build_and_Flash_with_VSCode.md)
- [Astra MCU SDK VS Code Extension User Guide](../../../docs/Astra_MCU_SDK_VSCode_Extension_User_Guide.md)

**Build (VS Code):**
1. Open **Build and Deploy** -> **Build Configurations**.
2. Select the **nand_sample_app** project configuration in the **Project Configuration** dropdown.
3. Select `sl2610_rdk_cm52_nand_sample_app_defconfig`.
4. If you need a different NAND test mode, update the application configuration before building.
5. Build with **Build (SDK+Project)** for the first build, or **Build (Project)** for rebuilds.

**Flash (VS Code):**
1. Use the SL2610 image-generation flow to generate the required sub-image.
2. Open **Image Flashing (SL2610)**.
3. Select **Flash Target** as **M52 Image**.
4. In **Image Path**, browse to and select the generated sub-image file, such as `sysmgr.subimg.gz`.
5. Start the flashing operation to program the image to the target.

---

## Building and Flashing the Example using CLI

Use the CLI flow described in the respective platform guide:
- [SL2610 Build and Flash with CLI](../../../docs/SL2610/SL2610_Build_and_Flash_with_CLI.md)
- [Astra MCU SDK User Guide](../../../docs/Astra_MCU_SDK_User_Guide.md)

**Build (CLI):**
1. Build from the application directory itself:
   ```bash
   cd <sdk-root>/examples/driver_examples/nand_sample_app
   export SRSDK_DIR=<sdk-root>
   make sl2610_rdk_cm52_nand_sample_app_defconfig BUILD=SRSDK
   ```
2. If you need to change the NAND test selection or related app configuration, apply the defconfig without building and then open `menuconfig`:
   ```bash
   cd <sdk-root>/examples/driver_examples/nand_sample_app
   export SRSDK_DIR=<sdk-root>
   make sl2610_rdk_cm52_nand_sample_app_defconfig BUILD=NONE
   make menuconfig
   ```
3. For faster rebuilds when only app code changes, reuse the app-local installed SDK package:
   ```bash
   cd <sdk-root>/examples/driver_examples/nand_sample_app
   export SRSDK_DIR=<sdk-root>
   make build
   ```
4. If this app has been exported to its own repository, use the same commands from that exported app directory after setting `SRSDK_DIR` to the SDK root.

**Build outputs (CLI):**
- Application binary: `<app-dir>/out/<target>/release/<target>.elf`
- App-local SDK package: `<app-dir>/install/<BOARD>/<BUILD_TYPE>/`

**Flash (CLI):**

1. Build the SL2610 bootloader image.
   ```bash
   cd <sdk-root>
   export SRSDK_DIR=<sdk-root>
   make <SL2610_Bootloader_defconfig> BOARD=<BOARD>
   make astrasdk
   ```

2. Generate the system sub-image.
   ```bash
   cd <sdk-root>/examples/driver_examples/nand_sample_app
   export SRSDK_DIR=<sdk-root>
   make imagegen
   ```

3. Flash or download the image using the SL2610 workflow.

   Refer: [SL2610 Build and Flash with CLI](../../../docs/SL2610/SL2610_Build_and_Flash_with_CLI.md)

---

## Running the Application using VS Code Extension

1. Complete the UART0 console connection from the **Connections** section.
2. Flash the image and press **RESET** on the board.
3. Open the serial console for the UART0 logger connection.
4. Observe NAND initialization logs, per-test pass/fail output, and the final application result.

**Expected Logs**

```text
0000000000:[0][INF][SYS ]:Application drivers initialization complete without errors.
0000000000:[0][INF][SYS ]:sl2610 SDK version 1.3.0
0000000000:[0][INF][FLSH]:
0000000002:[0][INF][FLSH]:NAND Flash Test Application
0000000007:[0][INF][FLSH]:Test 1: Flash Initialization
0000000012:[0][DBG][FLSH]:JEDEC ID: 0x42CDFF
0000000016:[0][DBG][FLSH]:Nand Randomizer is enabled!
0000000021:[0][INF][FLSH]:Init successful
0000000025:[0][INF][FLSH]:Test 1 PASSED: Flash Initialization
0000000030:[0][INF][FLSH]:Test 2: Read Device ID
0000000035:[0][DBG][FLSH]:JEDEC ID: 0x342CDFF
0000000039:[0][INF][FLSH]:JEDEC ID: 0x42CDFF [byte0=0xFF byte1=0xCD byte2=0x42]
0000000046:[0][INF][FLSH]:Read Device ID PASSED
0000000050:[0][INF][FLSH]:Test 2 PASSED: Read Device ID
0000000055:[0][INF][FLSH]:Test 5: NAND Page/Multipage Reads
0000000061:[0][INF][FLSH]:NAND Multi-Page Read (2 Pages)
0000000066:[0][DBG][FLSH]:Geo: PS=2048 OOB=64 PPB=64 BS=131072 | Read Addr=0x0 Len=2048
0000000074:[0][DBG][FLSH]:NAND Read: Addr=0x0, Len=2048
0000000081:[0][DBG][FLSH]:NAND Read: Addr=0x0, Block=0, Page=0, Col=0x0, Len=2048 logical_remaining 2048
0000000090:[0][DBG][FLSH]:Checking if block 0 is randomized (skip_randomized_blks: 16) status 1
0000000098:[0][DBG][FLSH]:Geo: PS=2048 OOB=64 PPB=64 BS=131072 | Read Addr=0x800 Len=2048
0000000106:[0][DBG][FLSH]:NAND Read: Addr=0x800, Len=2048
0000000113:[0][DBG][FLSH]:NAND Read: Addr=0x800, Block=0, Page=1, Col=0x0, Len=2048 logical_remaining 2048
0000000123:[0][DBG][FLSH]:Checking if block 0 is randomized (skip_randomized_blks: 16) status 1
0000000131:[0][INF][FLSH]:NAND Multi-Page Read SUCCESS: 2 pages (4096B)
0000000138:[0][INF][FLSH]:Test 5 PASSED: NAND Page/Multipage Reads
0000000144:[0][INF][FLSH]:NAND Sample Application PASSED
0000000149:[0][INF][FLSH]:NAND Sample Application Completed!
```
