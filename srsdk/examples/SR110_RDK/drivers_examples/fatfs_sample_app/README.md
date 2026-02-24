# FATFS Driver Sample Application

## Description

The FATFS Driver sample application demonstrates file-system operations on an SD card using FATFS over SDIO.

In simple terms, it verifies that the board can mount (or format and mount) an SD card, create folders/files, write data, read it back, rename directories, list directory contents, and unmount cleanly.

The sample flow includes:
- SDIO initialization for storage access.
- FATFS mount; if no filesystem is present, automatic FAT32 format and remount.
- Directory and file creation.
- File write/read verification.
- Directory rename and file access in renamed path.
- Directory listing and final unmount.

All key steps are logged so users can verify successful storage and file-system behavior.

## Hardware Requirements
- **Astra Machina Micro Kit (SR110) with SDK card slot**
- **MicroSD card**
- **SD card adapter card** (for connecting the SD card to the kit/slot as required)

## Prerequisites
- Choose **one** setup path:
  - **CLI**: [Setup and Install SDK using CLI](../../../../docs/Astra_MCU_SDK_Setup_and_Install_CLI.md)
  - **VS Code**: [Setup and Install SDK using VS Code](../../../../docs/Astra_MCU_SDK_Setup_and_Install_VsCode.md)

## Building and Flashing the Example using VS Code and CLI

Use the VS Code flow described in the SR110 guide and the VS Code Extension guide:
- [SR110 Build and Flash with VS Code](../../../../docs/SR110/SR110_Build_and_Flash_with_VSCode.md)
- [Astra MCU SDK VS Code Extension User Guide](../../../../docs/Astra_MCU_SDK_VSCode_Extension_User_Guide.md)

**Build (VS Code):**
1. Open **Build and Deploy** -> **Build Configurations**.
2. Select **fatfs_sample_app** in the **Application** dropdown.
3. Build with **Build (SDK + App)** for the first build, or **Build App** for rebuilds.

**Flash (VS Code):**
1. Use **Image Conversion** to generate the flash image.
2. Use **Image Flashing** (SWD/JTAG) to flash the firmware image.

Use the CLI flow described in the SR110 guide:
- [SR110 Build and Flash with CLI](../../../../docs/SR110/SR110_Build_and_Flash_with_CLI.md)

**Build (CLI):**
1. From `<sdk-root>/examples`, build the example:
   ```bash
   cd <sdk-root>/examples
   export SRSDK_DIR=<sdk-root>
   make cm55_fatfs_sample_app_defconfig BOARD=SR110_RDK BUILD=SRSDK
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

## Running the Application using VS Code Extension

1. **Before running the application, make sure to connect a USB cable to the Application SR110 USB port on the Astra Machina Micro board and then press the reset button**
   - For logging output, click on **SERIAL MONITOR** and connect to the DAP logger port.

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
