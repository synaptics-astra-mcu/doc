# eMMC Driver Sample Application

## Description

The eMMC Driver sample application demonstrates eMMC storage operations and data validation on the supported boards for this application. It performs comprehensive eMMC functionality testing including initialization, read/write operations, and data integrity verification to ensure reliable storage access.

The sample includes multiple eMMC operations:
- **eMMC initialization:** Initialize the eMMC storage interface and verify successful setup.
- **Write operations:** Write test data patterns to eMMC storage at specified addresses.
- **Read operations:** Read data back from eMMC storage for verification.
- **Data integrity validation:** Compare written and read data to ensure storage reliability.

During each run, the app logs initialization status, read/write operations, error codes, and validation results. This makes it easy for end users to confirm that eMMC setup and data operations are working as expected.

The latest example structure uses a **common application source tree** with board-specific hardware setup kept under `hw/<BOARD>/`. For this app:
- Common application sources such as `main.c`, `emmc_sample_app.c`, and `emmc_sample_app.h` stay in the app root.
- Application defconfigs are stored under `configs/`.
- Board and hardware-specific setup is selected from `hw/<BOARD>/`, for example `hw/SL2610_PEK/`.

The application can also be exported and built as a **standalone app repository**. In that flow, keep this app in its own directory, point `SRSDK_DIR` to the SDK root, and build from the app directory itself. For the full application workflow model, see [Astra MCU SDK User Guide](../../../docs/Astra_MCU_SDK_User_Guide.md).

## Supported Boards

This application supports:
- `SL2610_PEK`

Select the defconfig that matches your target board, and the build system will pick the corresponding board-specific hardware setup from `hw/<BOARD>/`.

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
- `sl2610_pek_cm52_emmc_defconfig`


## Building and Flashing the Example using VS Code

Use the VS Code flow described in the respective soc vscode guides and the VS Code Extension guide:
- [SL2610 Build and Flash with VS Code](../../../docs/SL2610/SL2610_Build_and_Flash_with_VSCode.md)
- [Astra MCU SDK VS Code Extension User Guide](../../../docs/Astra_MCU_SDK_VSCode_Extension_User_Guide.md)

**Build (VS Code):**
1. Open **Build and Deploy** -> **Build Configurations**.
2. Select the **eMMC_sample_app** project configuration in the **Project Configuration** dropdown.
3. Build with **Build (SDK+Project)** for the first build, or **Build (Project)** for rebuilds.

**Flash (VS Code):**
1. Use the SL2610 image-generation flow to generate the required sub-image.
2. Open **Image Flashing (SL2610)**.
3. Select **Flash Target** as **M52 Image**.
4. In **Image Path**, browse to and select the generated sub-image file, such as `sysmgr.subimg.gz`.
5. Start the flashing operation to program the image to the target.

---

## Building and Flashing the Example using CLI

Use the CLI flow described in the respective build guide:
- [SL2610 Build and Flash with CLI](../../../docs/SL2610/SL2610_Build_and_Flash_with_CLI.md)
- [Astra MCU SDK User Guide](../../../docs/Astra_MCU_SDK_User_Guide.md)

**Build (CLI):**
1. Build from the application directory itself:
   ```bash
   cd <sdk-root>/examples/driver_examples/eMMC_sample_app
   export SRSDK_DIR=<sdk-root>
   make <app_defconfig> BUILD=SRSDK
   ```
2. For faster rebuilds when only app code changes, reuse the app-local installed SDK package:
   ```bash
   cd <sdk-root>/examples/driver_examples/eMMC_sample_app
   export SRSDK_DIR=<sdk-root>
   make build
   ```
3. If this app has been exported to its own repository, use the same commands from that exported app directory after setting `SRSDK_DIR` to the SDK root.

**Build outputs (CLI):**
- Application binary: `<app-dir>/out/<target>/release/<target>.elf`
- App-local SDK package: `<app-dir>/install/<BOARD>/<BUILD_TYPE>/`

**Flash (CLI):**

1. Build the SL2610 bootloader image.
   ```
   cd <sdk-root>
   export SRSDK_DIR=<sdk-root>
   make <SL2610_Bootloader_defconfig> BOARD=<BOARD>
   make astrasdk
   ```

2. Generate the system sub-image.
   ```
   cd <sdk-root>/examples/driver_examples/eMMC_sample_app
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
3. eMMC sample logs appear in the logger window, including initialization status and read/write operations.

**Expected Logs**

```
0391468933:[0][WRN][LOGR]:Changing logger interface to LOGGER_IF_UART_1
0000000023:[0][INF][SYS ]:Application drivers initialization complete without errors.
0000004203:[0][INF][SYS ]:sr110 SDK version 1.3.0
0000006678:[0][INF][EMMC]:Starting EMMC Sample App! 
0000019167:[0][INF][EMMC]:EMMC Read/Write successfull 
0000021674:[0][INF][UART]:EMMC Sample App Completed!
```
