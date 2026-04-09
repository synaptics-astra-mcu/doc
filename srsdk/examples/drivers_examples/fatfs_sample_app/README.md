# FATFS Driver Sample Application

## Description

The FATFS Driver sample application demonstrates file-system operations on an SD card using FATFS over SDIO on the supported boards for this application. It performs comprehensive file-system testing including mounting, formatting, file operations, directory management, and unmounting to ensure reliable storage access.

The sample includes multiple file-system operations:
- **SDIO initialization:** Initialize SDIO interface for SD card access.
- **FATFS mount:** Mount existing filesystem or format and mount if no filesystem exists.
- **Directory operations:** Create directories and subdirectories for file organization.
- **File operations:** Create files, write data, and read back for verification.
- **Directory management:** Rename directories and access files in renamed paths.
- **Directory listing:** Enumerate directory contents to verify file-system structure.
- **Clean unmount:** Properly unmount the filesystem to ensure data integrity.

During each run, the app logs initialization status, mount operations, file creation, read/write verification, and unmount status. This makes it easy for end users to confirm that FATFS setup and file operations are working as expected.

The latest example structure uses a **common application source tree** with board-specific hardware setup kept under `hw/<BOARD>/`. For this app:
- Common application sources such as `main.c`, `fatfs_sample_app.c`, and `fatfs_sample_app.h` stay in the app root.
- Application defconfigs are stored under `configs/`.
- Board and hardware-specific setup is selected from `hw/<BOARD>/`, for example `hw/SR110_RDK/`.

The application can also be exported and built as a **standalone app repository**. In that flow, keep this app in its own directory, point `SRSDK_DIR` to the SDK root, and build from the app directory itself. For the full application workflow model, see [Astra MCU SDK User Guide](../../../docs/Astra_MCU_SDK_User_Guide.md).

## Supported Boards

This application supports:
- `SR110_RDK`

Select the defconfig that matches your target board, and the build system will pick the corresponding board-specific hardware setup from `hw/<BOARD>/`.

## Hardware Requirements
- **Astra Machina Micro Kit (SR110) with SDK card slot**
- **MicroSD card**
- **SD card adapter card** (for connecting SD card to the kit/slot as required)

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
- `sr110_rdk_cm55_fatfs_sample_app_defconfig`


## Building and Flashing the Example using VS Code

Use the VS Code flow described in the respective soc vscode guides and the VS Code Extension guide:
- [SR110 Build and Flash with VS Code](../../../docs/SR110/SR110_Build_and_Flash_with_VSCode.md)
- [Astra MCU SDK VS Code Extension User Guide](../../../docs/Astra_MCU_SDK_VSCode_Extension_User_Guide.md)

**Build (VS Code):**
1. Open **Build and Deploy** -> **Build Configurations**.
2. Select **fatfs_sample_app** in the **Application** dropdown.
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
   cd <sdk-root>/examples/driver_examples/fatfs_sample_app
   export SRSDK_DIR=<sdk-root>
   make <app_defconfig> BUILD=SRSDK
   ```
2. For faster rebuilds when only app code changes, reuse the app-local installed SDK package:
   ```bash
   cd <sdk-root>/examples/driver_examples/fatfs_sample_app
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
3. FATFS sample logs appear in the logger window, including file-system operations and validation results.

## Expected Logs

This section provides logs you might observe during the execution of the FATFS sample application, demonstrating successful SD card initialization and file operations.

```
0001398125:[0][INF][SYS ]:System initialization done
0002912772:[0][INF]:Speed getting set to SDR50
0003183726:[0][INF]:Clock speed set to 100Mhz!
0003347726:[0][DBG]:Result of SDIO Init Card = 0
0003411864:[0][INF]:Time for f_mount = 1681 msec
0003411893:[0][INF]:File system already exists, mounted successfully!
0003411920:[0][INF]:Mounting successful after formatting!
0004737359:[0][INF]:Speed getting set to SDR50
0004959726:[0][INF]:Clock speed set to 100Mhz!
0005127726:[0][DBG]:Result of SDIO Init Card = 0
0005198819:[0][INF]:Time for mkdir = 1472 msec
0005198848:[0][INF]:Created directory: mydir
0006471359:[0][INF]:Speed getting set to SDR50
0006721726:[0][INF]:Clock speed set to 100Mhz!
0006841726:[0][DBG]:Result of SDIO Init Card = 0
0006909070:[0][INF]:Created directory: mysubdir
0008117359:[0][INF]:Speed getting set to SDR50
0008364726:[0][INF]:Clock speed set to 100Mhz!
0008527726:[0][DBG]:Result of SDIO Init Card = 0
0008597324:[0][INF]:Cannot create new file
0008655823:[0][INF]:Time for f_open = 1478 msec
0008655852:[0][INF]:Main file opened!
0008656554:[0][INF]:Time for f_write = 0 msec
0008831523:[0][INF]:Main file written. Bytes: 22
0010049359:[0][INF]:Speed getting set to SDR50
0010311726:[0][INF]:Clock speed set to 100Mhz!
0010486726:[0][DBG]:Result of SDIO Init Card = 0
0010556078:[0][INF]:Cannot create new file
0010613981:[0][INF]:Subdir file open completed!
0010680957:[0][INF]:Subdir file written. Bytes: 25
0011892359:[0][INF]:Speed getting set to SDR50
0012151726:[0][INF]:Clock speed set to 100Mhz!
0012318726:[0][DBG]:Result of SDIO Init Card = 0
0012385758:[0][INF]:Existing file
0012386462:[0][INF]:Main file read: Hello from MAIN dir!

0013715359:[0][INF]:Speed getting set to SDR50
0013933726:[0][INF]:Clock speed set to 100Mhz!
0014098726:[0][DBG]:Result of SDIO Init Card = 0
0014178821:[0][INF]:Existing file
0014179525:[0][INF]:Time for f_read = 0 msec
0014179551:[0][INF]:Subdir file read: Hello from SUBDIR file!

0014179608:[0][INF]:List of files in mydir:
0015617359:[0][INF]:Speed getting set to SDR50
0015879726:[0][INF]:Clock speed set to 100Mhz!
0016000726:[0][DBG]:Result of SDIO Init Card = 0
0016070002:[0][INF]:[DIR]  MYSUBDIR
0016070045:[0][INF]:[FILE] MAIN.TXT (22 bytes)
0016070104:[0][INF]:List of files in mysubdir:
0017401359:[0][INF]:Speed getting set to SDR50
0017658726:[0][INF]:Clock speed set to 100Mhz!
0017821726:[0][DBG]:Result of SDIO Init Card = 0
0017890068:[0][INF]:[FILE] SUB_FILE.TXT (25 bytes)
0017890130:[0][INF]:Unmount complete!

0017890154:[0][INF]:FATFS Sample App Completed! 
```
