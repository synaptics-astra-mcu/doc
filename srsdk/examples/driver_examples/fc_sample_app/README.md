# FC Driver Sample Application

## Description

The FC Driver sample application demonstrates Fault Controller operation on the supported boards for this application. It performs FC controller initialization, real fault-source routing, interrupt validation, DMA trigger validation, reset validation, FC log capture checks, and callback completion checks.

The sample includes multiple FC operations:
- **FC controller initialization:** Initialize the selected FC instance and configure interrupt, reset, trigger, timing, and fault-association routes.
- **Watchdog fault validation:** Generate a real watchdog fault source and validate FC interrupt, DMA trigger, log capture, and reset routing.
- **PPC fault validation:** Generate a PPC violation and validate FC interrupt, DMA trigger, log capture, and reset routing.
- **System-fault validation:** Generate a recoverable CM52 UsageFault and validate FC interrupt, DMA trigger, log capture, and reset routing.
- **DMA trigger validation:** Arm DMA on an FC trigger request, generate the selected fault source, and validate DMA completion and copied data.
- **Reset-route validation:** Arm an FC reset route, generate the selected fault source, reboot, and validate retained reset state after restart.
- **Runtime checks:** Validate ISR completion, callback status, interrupt status bits, FC log count, FC log ID, DMA wait-for-trigger status, DMA done status, and final pass/fail status.

FC callbacks execute in ISR context. Reset-route tests intentionally reboot the target while retained state is used to validate that the FC reset route fired.

The latest example structure uses a **common application source tree** with board-specific hardware setup kept under `hw/<BOARD>/`. For this app:
- Common application sources such as `main.c`, `fc_sample_app.c`, and `fc_sample_app.h` stay in the app root.
- Application defconfigs are stored under `configs/`.
- Board and hardware-specific setup is selected from `hw/<BOARD>/`, for example `hw/SL2610_PEK/` and `hw/SL2610_RDK/`.

The application can also be exported and built as a **standalone app repository**. In that flow, keep this app in its own directory, point `SRSDK_DIR` to the SDK root, and build from the app directory itself. For the full application workflow model, see [Astra MCU SDK User Guide](../../../docs/Astra_MCU_SDK_User_Guide.md).

## Supported Boards

This application supports:
- `SL2610_PEK`
- `SL2610_RDK`

Select the defconfig that matches your target board, and the build system will pick the corresponding board-specific hardware setup from `hw/<BOARD>/`.

## Hardware Requirements

The FC sample uses on-chip fault sources and does not require external FC wiring.

The watchdog, PPC, and system-fault reset validation flows intentionally reset the board. Use these presets only when the target can be reset as part of the validation flow.

## Prerequisites
- Choose **one** setup path:
  - **CLI**: [Setup and Install SDK using CLI](../../../docs/Astra_MCU_SDK_Setup_and_Install_CLI.md)
  - **VS Code**: [Setup and Install SDK using VS Code](../../../docs/Astra_MCU_SDK_Setup_and_Install_VsCode.md)
- The current FC sample app package flow supports **GCC builds only**. AC6/LLVM are not currently validated for this sample.

## Test Case Selection

Before building, choose the testcase defconfig that matches your target board.

You can:
- Select the required defconfig directly from the application's `configs/` directory.
- Run `make list_defconfigs` from the application directory to list all supported defconfigs.

Current FC sample presets are:
- `sl2610_pek_cm52_fc_wd_defconfig`
- `sl2610_pek_cm52_fc_ppc_defconfig`
- `sl2610_pek_cm52_fc_system_fault_defconfig`
- `sl2610_rdk_cm52_fc_wd_defconfig`
- `sl2610_rdk_cm52_fc_ppc_defconfig`
- `sl2610_rdk_cm52_fc_system_fault_defconfig`

For this app, the default defconfig is:
   - `sl2610_pek_cm52_fc_wd_defconfig`

The presets map to the following primary validation flows:
- `sl2610_pek_cm52_fc_wd_defconfig`
  Testcase: `CONFIG_APP_FC_TEST_CASE_WD`
  Coverage: watchdog FC interrupt validation, watchdog FC DMA-trigger validation, and watchdog FC reset validation.
- `sl2610_pek_cm52_fc_ppc_defconfig`
  Testcase: `CONFIG_APP_FC_TEST_CASE_PPC`
  Coverage: PPC violation FC interrupt validation, PPC violation FC DMA-trigger validation, and PPC violation FC reset validation.
- `sl2610_pek_cm52_fc_system_fault_defconfig`
  Testcase: `CONFIG_APP_FC_TEST_CASE_SYS_EXCEPTION`
  Coverage: recoverable CM52 system-fault FC interrupt validation, system-fault FC DMA-trigger validation, and system-fault FC reset validation.
- `sl2610_rdk_cm52_fc_wd_defconfig`
  Testcase: `CONFIG_APP_FC_TEST_CASE_WD`
  Coverage: watchdog FC interrupt validation, watchdog FC DMA-trigger validation, and watchdog FC reset validation.
- `sl2610_rdk_cm52_fc_ppc_defconfig`
  Testcase: `CONFIG_APP_FC_TEST_CASE_PPC`
  Coverage: PPC violation FC interrupt validation, PPC violation FC DMA-trigger validation, and PPC violation FC reset validation.
- `sl2610_rdk_cm52_fc_system_fault_defconfig`
  Testcase: `CONFIG_APP_FC_TEST_CASE_SYS_EXCEPTION`
  Coverage: recoverable CM52 system-fault FC interrupt validation, system-fault FC DMA-trigger validation, and system-fault FC reset validation.

FC validation options can be tuned in menuconfig:
- Watchdog source and route parameters:
  - `CONFIG_APP_FC_WD_INSTANCE`
  - `CONFIG_APP_FC_WD_RST_FAULT_ID`
  - `CONFIG_APP_FC_WD_INTERRUPT_ID`
  - `CONFIG_APP_FC_WD_RESET_ID`
- PPC source and route parameters:
  - `CONFIG_APP_FC_PPC_TARGET_BIT`
  - `CONFIG_APP_FC_PPC_TARGET_ADDR`
  - `CONFIG_APP_FC_PPC_INTERRUPT_ID`
  - `CONFIG_APP_FC_PPC_RESET_ID`
- System-fault source and route parameters:
  - `CONFIG_APP_FC_SYS_EXCEPTION_FAULT_ID`
  - `CONFIG_APP_FC_SYS_EXCEPTION_INTERRUPT_ID`
  - `CONFIG_APP_FC_SYS_EXCEPTION_RESET_ID`
- DMA trigger parameters:
  - `CONFIG_APP_FC_PPC_DMA_TRIGGER_REQ_ID`
  - `CONFIG_APP_FC_WD_DMA_TRIGGER_REQ_ID`
  - `CONFIG_APP_FC_SYS_EXCEPTION_DMA_TRIGGER_REQ_ID`
  - `CONFIG_APP_FC_DMA_TIMEOUT_LOOPS`

## Logger Interface Configuration

The checked-in FC defconfigs enable the UART0 logger:
- `CONFIG_LOGGER_IF_UART_0=y`

Observe logs on the UART0 console unless you intentionally change the logger configuration.

## Connections

### General host connections

- Power the board using the designated power adapter.
- Connect the UART0 slot to the host machine using a UART cable for serial logs.
- Connect the USB2 slot to the host machine if you are using the USB image-loading workflow.

### Single-board validation

The FC validation presets use on-chip sources and do not require external signal wiring:
- `sl2610_pek_cm52_fc_wd_defconfig`
- `sl2610_pek_cm52_fc_ppc_defconfig`
- `sl2610_pek_cm52_fc_system_fault_defconfig`
- `sl2610_rdk_cm52_fc_wd_defconfig`
- `sl2610_rdk_cm52_fc_ppc_defconfig`
- `sl2610_rdk_cm52_fc_system_fault_defconfig`

### Reset-route validation

All FC presets include reset-route validation. The final step arms an FC reset output and intentionally resets the target:
- Watchdog reset validation uses the configured watchdog fault source.
- PPC reset validation uses the configured PPC violation source.
- System-fault reset validation uses the configured recoverable CM52 UsageFault source.

After reset, the application resumes and reports the retained reset validation result on UART0.

## Building and Flashing the Example using VS Code

Use the VS Code flow described in the respective soc vscode guides and the VS Code Extension guide:
- [SL2610 Build and Flash with VS Code](../../../docs/SL2610/SL2610_Build_and_Flash_with_VSCode.md)
- [Astra MCU SDK VS Code Extension User Guide](../../../docs/Astra_MCU_SDK_VSCode_Extension_User_Guide.md)

**Build (VS Code):**
1. Open **Build and Deploy** -> **Build Configurations**.
2. Select the **fc_sample_app** project configuration in the **Project Configuration** dropdown.
3. Select the testcase defconfig that matches the flow you want to validate.
4. Build with **Build (SDK+Project)** for the first build, or **Build (Project)** for rebuilds.

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
   cd <sdk-root>/examples/driver_examples/fc_sample_app
   export SRSDK_DIR=<sdk-root>
   make clean
   make <app_defconfig> BUILD=SRSDK
   ```
2. For faster rebuilds when only app code changes, reuse the app-local installed SDK package:
   ```bash
   cd <sdk-root>/examples/driver_examples/fc_sample_app
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
   ```bash
   cd <sdk-root>
   export SRSDK_DIR=<sdk-root>
   make <SL2610_Bootloader_defconfig> BOARD=<BOARD>
   make astrasdk
   ```

2. Generate the system sub-image.
   ```bash
   cd <sdk-root>/examples/driver_examples/fc_sample_app
   export SRSDK_DIR=<sdk-root>
   make imagegen
   ```

3. Flash/download image to target.

   Refer: [SL2610 Platform Guide](../../../docs/SL2610/SL2610_Build_and_Flash_with_CLI.md)

---

## Running the Application using VS Code Extension

1. Press **RESET** on the board after flashing.
2. For logging output, click **SERIAL MONITOR** and connect to the logger port.
   - The logger port is not guaranteed to be consistent across OSes.
   - If you do not see logs after a reset, switch to the other detected logger port.
3. FC sample logs appear in the logger window, including testcase selection, FC route configuration, ISR status, DMA trigger status, reset-route status, and final pass/fail status.

### Sample applications logs and output

1. **Watchdog FC Application**
   - Console logs.
   ```bash
   FC validation: Watchdog interrupt + DMA trigger + Reset
   ============================================================
   FC Watchdog ISR validation
   ============================================================
   FC Watchdog ISR PASS
   FC Watchdog DMA trigger PASS
   FC Watchdog Reset PASS: reboot observed while fault_id=5 reset_id=0 route remained armed
   FC validation PASS
   ```

2. **PPC FC Application**
   - Console logs.
   ```bash
   FC validation: PPC interrupt + DMA trigger + Reset
   ============================================================
   FC PPC ISR validation
   ============================================================
   FC PPC ISR PASS
   FC PPC DMA trigger PASS
   FC PPC Reset PASS: reboot observed while reset output was armed
   FC validation PASS
   ```

3. **System Fault FC Application**
   - Console logs.
   ```bash
   FC validation: System fault interrupt + DMA trigger + Reset
   ============================================================
   FC System fault ISR validation
   ============================================================
   FC System fault ISR PASS
   FC System fault DMA trigger PASS
   FC System fault Reset PASS: reboot observed while reset output was armed
   FC validation PASS
   ```
