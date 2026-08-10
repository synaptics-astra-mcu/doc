# I3C Driver Sample Application

## Description

The I3C Driver sample application demonstrates I3C controller-mode and target-mode validation on the supported boards for this application. It covers I3C bus bring-up, dynamic address assignment, directed CCC handling, blocking and non-blocking transfers, In-Band Interrupt (IBI), Master Request (MR), Hot-Join, I2C compatibility mode, and speed-test validation.

The sample includes multiple I3C validation flows:
- **Controller/target mode validation:** Build controller-mode or target-mode images from the same sample app.
- **Bus management tests:** Validate DAA and directed CCC flows such as `SETDASA`, `SETNEWDA`, and `SETMWL`.
- **Transfer tests:** Exercise blocking and non-blocking read/write transfers between controller and target.
- **IBI / MR / Hot-Join:** Validate interrupt-style I3C flows and controller-target coordination.
- **I2C compatibility mode:** Validate blocking transfer flow with an I2C-target-compatible configuration.
- **Speed test:** Measure and validate I3C traffic for the selected speed and transfer direction.
- **IIM4235 feature validation:** Run controller-only validation for blocking, non-blocking, and IBI interactions with the IIM4235 flow.

During each run, the app logs initialization status, active mode, testcase execution, and pass/fail results. This makes it easy for end users to confirm that I3C setup and protocol behavior are working as expected.

The latest example structure uses a **common application source tree** with board-specific hardware setup kept under `hw/<BOARD>/`. For this app:
- Common application sources such as `main.c`, `i3c_sample_app.c`, and `i3c_sample_app.h` stay in the app root.
- Application defconfigs are stored under `configs/`.
- Board and hardware-specific setup is selected from `hw/<BOARD>/`, for example `hw/SL2610_PEK/`.

The application can also be exported and built as a **standalone app repository**. In that flow, keep this app in its own directory, point `SRSDK_DIR` to the SDK root, and build from the app directory itself. For the full application workflow model, see [Astra MCU SDK User Guide](../../../docs/Astra_MCU_SDK_User_Guide.md).

## Supported Boards

This application supports:
- `SL2610_PEK`

Select the defconfig that matches your target board and required I3C validation flow, and the build system will pick the corresponding board-specific hardware setup from `hw/<BOARD>/`.

## Prerequisites
- Choose **one** setup path:
  - **CLI**: [Setup and Install SDK using CLI](../../../docs/Astra_MCU_SDK_Setup_and_Install_CLI.md)
  - **VS Code**: [Setup and Install SDK using VS Code](../../../docs/Astra_MCU_SDK_Setup_and_Install_VsCode.md)

## Project Configuration Selection

Before building, choose the project configuration (defconfig) that matches the validation flow you want to run.

You can:
- Select the required defconfig directly from the application's `configs/` directory.
- Run `make list_defconfigs` from the application directory to list all supported defconfigs.

**Available I3C defconfigs (`SL2610_PEK`):**
- `sl2610_pek_cm52_i3c_rw_transfer_defconfig`: controller-side blocking read/write transfer validation.
- `sl2610_pek_cm52_i3c_setdasa_defconfig`: controller-side `DAA -> SETDASA -> blocking write` flow.
- `sl2610_pek_cm52_i3c_setnewda_defconfig`: controller-side `DAA -> SETNEWDA -> blocking write` flow.
- `sl2610_pek_cm52_i3c_setmwl_defconfig`: controller-side `DAA -> SETMWL -> blocking write` flow.
- `sl2610_pek_cm52_i3c_ibi_defconfig`: controller-side IBI receive validation.
- `sl2610_pek_cm52_i3c_mr_defconfig`: controller-side master-request wait validation.
- `sl2610_pek_cm52_i3c_hot_join_defconfig`: controller-side Hot-Join wait and enumeration validation.
- `sl2610_pek_cm52_i3c_speed_defconfig`: controller-side speed-test validation.
- `sl2610_pek_cm52_i3c_i2c_compatibility_defconfig`: controller-side I2C-compatible blocking transfer validation.
- `sl2610_pek_cm52_i3c_iim4235_all_features_defconfig`: controller-only IIM4235 blocking, non-blocking, and IBI validation.

For this app, the default defconfig is:
   - `sl2610_pek_cm52_i3c_rw_transfer_defconfig`

## Mode Configuration (Important)

The checked-in I3C defconfigs build the **controller image** by default:
- `CONFIG_APP_I3C_CONTROLLER_MODE=y`
- `# CONFIG_APP_I3C_TARGET_MODE is not set`

### Single Board validation

Use:
- `sl2610_pek_cm52_i3c_iim4235_all_features_defconfig`

This preset is intended for controller-only IIM4235 validation on a single board.

### Two-board validation

The following presets are intended for controller-to-target validation across two boards:
- `sl2610_pek_cm52_i3c_setdasa_defconfig`: target reads controller write after `SETDASA`.
- `sl2610_pek_cm52_i3c_setnewda_defconfig`: target reads controller write after `SETNEWDA`.
- `sl2610_pek_cm52_i3c_setmwl_defconfig`: target reads controller write after `SETMWL`.
- `sl2610_pek_cm52_i3c_rw_transfer_defconfig`: controller write/read with matching target read/write flow.
- `sl2610_pek_cm52_i3c_ibi_defconfig`: controller receives IBI and target generates IBI.
- `sl2610_pek_cm52_i3c_mr_defconfig`: controller waits for MR and target generates MR.
- `sl2610_pek_cm52_i3c_hot_join_defconfig`: controller waits for Hot-Join and target enters wait mode with Hot-Join enabled.
- `sl2610_pek_cm52_i3c_i2c_compatibility_defconfig`: controller and target validate the I2C-compatible transfer path.
- `sl2610_pek_cm52_i3c_speed_defconfig`: controller runs the selected speed/direction test and target provides the matching peer behavior.

For two-board validation:
1. Build the controller image with the selected testcase defconfig.
2. Re-apply the same testcase defconfig.
3. Switch the application to target mode in `menuconfig`.
4. If you want to validate the **non-blocking** transfer path, change the testcase in `menuconfig` under:
   ```text
   Application Configuration  --->  I3C Test Configuration  --->  I3C Test Case
   ```
   Select:
   - `I3C_TEST_CASE_RW_NON_BLOCKING` for standard I3C controller/target non-blocking read/write validation.
   - `I3C_TEST_CASE_I2C_RW_NON_BLOCKING` for the I2C-compatible non-blocking validation flow.
5. Rebuild a second image for the target board.

Equivalent target-mode `.config` change:

```text
# CONFIG_APP_I3C_CONTROLLER_MODE is not set
CONFIG_APP_I3C_TARGET_MODE=y
```

### Speed-test configuration

When using `sl2610_pek_cm52_i3c_speed_defconfig`, also select the required speed-test options in Kconfig.

Select one speed:

```text
CONFIG_APP_I3C_SPEED_TEST_SDR0
CONFIG_APP_I3C_SPEED_TEST_SDR1
CONFIG_APP_I3C_SPEED_TEST_SDR2
CONFIG_APP_I3C_SPEED_TEST_SDR3
CONFIG_APP_I3C_SPEED_TEST_SDR4
CONFIG_APP_I3C_SPEED_TEST_HDR_TS
CONFIG_APP_I3C_SPEED_TEST_HDR_DDR
```

Select one direction:

```text
CONFIG_APP_I3C_SPEED_TEST_READ
CONFIG_APP_I3C_SPEED_TEST_WRITE
```

Direction mapping:
- `CONFIG_APP_I3C_SPEED_TEST_READ`: controller performs read, target prepares write.
- `CONFIG_APP_I3C_SPEED_TEST_WRITE`: controller performs write, target prepares read.

### Hot-Join note

For `sl2610_pek_cm52_i3c_hot_join_defconfig`, load the controller image first and then load the target image.

## Logger Interface Configuration

The checked-in I3C defconfigs enable the UART0 logger:
- `CONFIG_LOGGER_IF_UART_0=y`

Observe logs on the UART0 console unless you intentionally change the logger configuration.

## Connections

### General host connections

- Power the board using the designated power adapter.
- Connect the **UART0** slot to the host machine using a UART cable for serial logs.
- Connect the **USB2** slot to the host machine if you are using the USB image-loading workflow.

### Single-board validation

For standalone testing and protocol monitoring:
- Connect a logic analyzer to the designated I3C master pins.
- Use `J15 I3C_MST` and `J16 I3C_MST`.
- On `J15`, the 2nd and 3rd positions correspond to `SDA` and `SCL`.
- On `J16`, the 4th position is used for `GND`.

### Two-board validation

For controller-to-target communication testing, connect the two `SL2610_PEK` boards together:
- `SDA -> SDA`
- `SCL -> SCL`
- `GND -> GND`

Ensure both boards share a common ground to maintain signal integrity and prevent communication failures.

## Building and Flashing the Example using VS Code

Use the VS Code flow described in the respective platform guide and the VS Code Extension guide:
- [SL2610 Build and Flash with VS Code](../../../docs/SL2610/SL2610_Build_and_Flash_with_VSCode.md)
- [Astra MCU SDK VS Code Extension User Guide](../../../docs/Astra_MCU_SDK_VSCode_Extension_User_Guide.md)

**Build (VS Code):**
1. Open **Build and Deploy** -> **Build Configurations**.
2. Select the **i3c_sample_app** project configuration in the **Project Configuration** dropdown.
3. Select the testcase defconfig that matches the flow you want to validate.
4. For two-board validation, build the controller image first, then rebuild a second image after switching the app configuration to target mode.
5. For speed-test validation, update the speed and direction options in the app configuration before building.
6. Build with **Build (SDK+Project)** for the first build, or **Build (Project)** for rebuilds.

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
1. Build the controller image from the application directory itself:
   ```bash
   cd <sdk-root>/examples/driver_examples/i3c_sample_app
   export SRSDK_DIR=<sdk-root>
   make <i3c_testcase_defconfig> BUILD=SRSDK
   ```
2. For two-board validation, rebuild the same testcase as a target image:
   ```bash
   cd <sdk-root>/examples/driver_examples/i3c_sample_app
   export SRSDK_DIR=<sdk-root>
   make <i3c_testcase_defconfig> BUILD=NONE
   make menuconfig
   make build
   ```
3. In `menuconfig`, switch:
   ```text
   I3C Operating Mode  --->  I3C_TARGET_MODE
   ```
4. For `sl2610_pek_cm52_i3c_speed_defconfig`, select the required speed and direction in `menuconfig` before the final build.
5. For faster rebuilds when only app code changes, reuse the app-local installed SDK package:
   ```bash
   cd <sdk-root>/examples/driver_examples/i3c_sample_app
   export SRSDK_DIR=<sdk-root>
   make build
   ```
6. If this app has been exported to its own repository, use the same commands from that exported app directory after setting `SRSDK_DIR` to the SDK root.

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
   cd <sdk-root>/examples/driver_examples/i3c_sample_app
   export SRSDK_DIR=<sdk-root>
   make imagegen
   ```

3. Load the generated image to the target using one of the following workflows.

**USB drive flash workflow**

Ensure eMMC is already flashed on the device, then:
1. Copy `sysmgr.subimg.gz` to a USB pendrive.
2. Insert the USB pendrive into the device.
3. In the U-Boot console, run:
   ```text
   img2sd usbh sysmgr.subimg.gz sd6
   ```

**USB boot tool RAM-execution workflow**

1. Copy the shared `preboot.subimg.gz` to a USB drive and flash it to partition `b1`:
   ```text
   img2sd usbh preboot.subimg.gz b1
   ```
2. Reset the board so it loads the preboot image with USB boot support.
3. Extract `sysmgr.subimg.gz` to `sysmgr.subimg`.
4. Update the provided USB boot script with:
   - The COM port connected to the device.
   - The path to `sysmgr.subimg`.
5. Run the script:
   ```bash
   python flash_script.py
   ```

For other supported loading methods, refer to the [SL2610 Build and Flash with CLI](../../../docs/SL2610/SL2610_Build_and_Flash_with_CLI.md) guide.

---

## Running the Application

1. Complete the required board connections from the **Connections** section.
2. Load the controller image to the first board.
3. For two-board validation, load the matching target image to the second board.
4. Open the UART0 serial console to monitor logs.
5. Reset the board or boards and observe the testcase execution logs.

For two-board validation:
- Reset both boards after wiring is complete.
- For Hot-Join validation, ensure the controller board is loaded first.
