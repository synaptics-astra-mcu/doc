# I2C Expander Sample Application

## Description

The I2C Expander sample application demonstrates I2C communication and GPIO control through I2C expanders on the supported boards for this application. It performs continuous GPIO toggling on I2C expander pins to verify reliable I2C communication and GPIO expansion functionality.

The sample includes multiple I2C operations:
- **I2C initialization:** Initialize I2C interface and configure communication with expander devices.
- **Expander detection:** Support for multiple I2C expander types (FXL6408, TCA6416A).
- **GPIO configuration:** Configure GPIO pins on the expander for output operations.
- **Continuous toggling:** Toggle GPIO pin 5 between high (1) and low (0) states with configurable delay.
- **Multi-device support:** Control multiple I2C expanders simultaneously if configured.

During each run, the app logs I2C initialization, expander detection, GPIO state changes, and operation results. This makes it easy for end users to confirm that I2C setup and GPIO expansion are working as expected.

The latest example structure uses a **common application source tree** with board-specific hardware setup kept under `hw/<BOARD>/`. For this app:
- Common application sources such as `main.c`, `i2c_exp_app.c`, and `i2c_exp_app.h` stay in the app root.
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

Before building, choose the project configuration (defconfig) that matches your target board.

You can:
- Select the required defconfig directly from the application's `configs/` directory.
- Run `make list_defconfigs` from the application directory to list all supported defconfigs.

**Available defconfigs:**
- `sl2610_rdk_cm52_i2c_exp_app_defconfig`


For this app, the default defconfig is:
   - `sl2610_rdk_cm52_i2c_exp_app_defconfig`

## Building and Flashing the Example using VS Code

Use the VS Code flow described in the respective soc vscode guides and the VS Code Extension guide:
- [SL2610 Build and Flash with VS Code](../../../docs/SL2610/SL2610_Build_and_Flash_with_VSCode.md)
- [Astra MCU SDK VS Code Extension User Guide](../../../docs/Astra_MCU_SDK_VSCode_Extension_User_Guide.md)

**Build (VS Code):**
1. Open **Build and Deploy** -> **Build Configurations**.
2. Select the **i2c_exp_app** project configuration in the **Project Configuration** dropdown.
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
   cd <sdk-root>/examples/driver_examples/i2c_exp_app
   export SRSDK_DIR=<sdk-root>
   make <app_defconfig> BUILD=SRSDK
   ```
2. For faster rebuilds when only app code changes, reuse the app-local installed SDK package:
   ```bash
   cd <sdk-root>/examples/driver_examples/i2c_exp_app
   export SRSDK_DIR=<sdk-root>
   make build
   ```
3. If this app has been exported to its own repository, use the same commands from that exported app directory after setting `SRSDK_DIR` to the SDK root.

**Build outputs (CLI):**
- Application binary: `<app-dir>/out/<target>/release/<target>.elf`
- App-local SDK package: `<app-dir>/install/<BOARD>/<BUILD_TYPE>/`

**Flash (CLI):**

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
   cd <sdk-root>/examples/driver_examples/i2c_exp_app
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
3. I2C expander logs appear in the logger window, including GPIO toggle status and I2C communication results.

**Expected Logs**

```
Init DDR4 @ 3200
DHL:v0p40 PT:v0p40 ID:0x21010000
USB MOUNTED
0000000000:[0][WRN][LOGR]:Changing logger interface to LOGGER_IF_UART_0
0000000000:[0][INF][SYS ]:M52:: Build Date 22-01-2026 Time 10:59:39 Commit unknown
0000000000:[0][INF][GENR]:I2C Expander GPIO Toggle Task Started
0000000005:[0][INF][GENR]:Controlling FXL6408 pin 5
0000000010:[0][INF][GENR]:FXL6408 pin 5 set to 0
0000001015:[0][INF][GENR]:FXL6408 pin 5 set to 1
0000002019:[0][INF][GENR]:FXL6408 pin 5 set to 0
0000003023:[0][INF][GENR]:FXL6408 pin 5 set to 1
0000004027:[0][INF][GENR]:FXL6408 pin 5 set to 0
0000005031:[0][INF][GENR]:FXL6408 pin 5 set to 1
0000006035:[0][INF][GENR]:FXL6408 pin 5 set to 0
0000007039:[0][INF][GENR]:FXL6408 pin 5 set to 1
0000008043:[0][INF][GENR]:FXL6408 pin 5 set to 0
0000009047:[0][INF][GENR]:FXL6408 pin 5 set to 1
0000010051:[0][INF][GENR]:FXL6408 pin 5 set to 0
0000011055:[0][INF][GENR]:FXL6408 pin 5 set to 1
0000012059:[0][INF][GENR]:FXL6408 pin 5 set to 0
0000013063:[0][INF][GENR]:FXL6408 pin 5 set to 1
0000014067:[0][INF][GENR]:FXL6408 pin 5 set to 0
0000015071:[0][INF][GENR]:FXL6408 pin 5 set to 1
0000016075:[0][INF][GENR]:FXL6408 pin 5 set to 0

```
