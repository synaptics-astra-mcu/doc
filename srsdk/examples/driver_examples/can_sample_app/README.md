# CAN Driver Sample Application

## Description

The CAN Driver sample application demonstrates Classical CAN and CAN FD operation on the supported boards for this application. It performs controller initialization, internal controller loopback validation, acceptance-filter validation, scheduled transmit checks, timestamp checks, and CAN FD payload/DLC matrix validation.

The sample includes multiple CAN operations:
- **CAN controller initialization:** Initialize the selected CAN instance and configure bitrate, sample point, framing mode, ACK behavior, ECC, watchdog, and retransmission settings.
- **Internal controller loopback:** Validate Classical CAN, CAN FD, and RTR traffic without requiring an external CAN bus.
- **Acceptance filtering:** Validate exact, masked, and accept-all receive filter behavior.
- **Scheduled transmit:** Queue frames for timestamp-based transmission and validate scheduled TX timing.
- **CAN FD payload/DLC matrix:** Validate legal CAN FD payload lengths of 0-8, 12, 16, 20, 24, 32, 48, and 64 bytes.
- **High-rate TX/RX burst validation:** Exercise repeated transmit and receive traffic across configured frame counts and delays.
- **Runtime checks:** Validate ISO/non-ISO FD framing, bus error-state handling, callback completion, and timestamp monotonicity.

CAN callbacks execute in ISR context. RX frame pointers are valid only during the RX callback.

The latest example structure uses a **common application source tree** with board-specific hardware setup kept under `hw/<BOARD>/`. For this app:
- Common application sources such as `main.c`, `can_sample_app.c`, and `can_sample_app.h` stay in the app root.
- Application defconfigs are stored under `configs/`.
- Board and hardware-specific setup is selected from `hw/<BOARD>/`, for example `hw/SL2610_PEK/`.

The application can also be exported and built as a **standalone app repository**. In that flow, keep this app in its own directory, point `SRSDK_DIR` to the SDK root, and build from the app directory itself. For the full application workflow model, see [Astra MCU SDK User Guide](../../../docs/Astra_MCU_SDK_User_Guide.md).

## Supported Boards

This application supports:
- `SL2610_PEK`

Select the defconfig that matches your target board, and the build system will pick the corresponding board-specific hardware setup from `hw/<BOARD>/`.

## Hardware Requirements

Internal controller loopback testcases do not require an external CAN bus.

External dual-instance validation requires a board setup that connects CAN0 transmit/receive paths to CAN1 and should only be used when that hardware path is available. The standalone dual-instance preset and the final dual-instance step inside functional coverage both require this external route.

## Prerequisites
- Choose **one** setup path:
  - **CLI**: [Setup and Install SDK using CLI](../../../docs/Astra_MCU_SDK_Setup_and_Install_CLI.md)
  - **VS Code**: [Setup and Install SDK using VS Code](../../../docs/Astra_MCU_SDK_Setup_and_Install_VsCode.md)
- The current CAN sample app package flow supports **GCC builds only**. AC6/LLVM are not currently validated for this sample.

## Project Configuration Selection

Before building, choose the project configuration (defconfig) that matches your target board.

You can:
- Select the required defconfig directly from the application's `configs/` directory.
- Run `make list_defconfigs` from the application directory to list all supported defconfigs.

Current CAN sample presets are:
- `sl2610_pek_cm52_can_classic_loopback_defconfig`
- `sl2610_pek_cm52_can_fd_loopback_defconfig`
- `sl2610_pek_cm52_can_classic_rtr_loopback_defconfig`
- `sl2610_pek_cm52_can_scheduled_tx_defconfig`
- `sl2610_pek_cm52_can_rx_accept_all_defconfig`
- `sl2610_pek_cm52_can_dual_instance_defconfig`
- `sl2610_pek_cm52_can_functional_coverage_defconfig`

For this app, the default defconfig is:
   - `sl2610_pek_cm52_can_classic_loopback_defconfig`

The presets map to the following primary validation flows:
- `sl2610_pek_cm52_can_classic_loopback_defconfig`
  Testcase: `CONFIG_APP_CAN_TEST_CASE_LOOPBACK_TX`
  Coverage: Classical CAN internal controller loopback, single-frame or DLC sweep, standard or extended ID.
- `sl2610_pek_cm52_can_fd_loopback_defconfig`
  Testcase: `CONFIG_APP_CAN_TEST_CASE_LOOPBACK_FD`
  Coverage: CAN FD internal controller loopback, single-frame or payload/DLC matrix, standard or extended ID.
- `sl2610_pek_cm52_can_classic_rtr_loopback_defconfig`
  Testcase: `CONFIG_APP_CAN_TEST_CASE_CLASSIC_RTR_STD_EXT`
  Coverage: Classical CAN RTR internal controller loopback validation.
- `sl2610_pek_cm52_can_scheduled_tx_defconfig`
  Testcase: `CONFIG_APP_CAN_TEST_CASE_SCHEDULED_TX_LOOPBACK`
  Coverage: scheduled transmit loopback across configured frame count and delay.
- `sl2610_pek_cm52_can_rx_accept_all_defconfig`
  Testcase: `CONFIG_APP_CAN_TEST_CASE_RX_ACCEPT_ALL_LOOPBACK`
  Coverage: accept-all filter behavior for Classical CAN and CAN FD paths.
- `sl2610_pek_cm52_can_dual_instance_defconfig`
  Testcase: `CONFIG_APP_CAN_TEST_CASE_DUAL_INSTANCE_TX_RX`
  Coverage: CAN0 to CAN1 external dual-instance transfer validation when the board is wired for that path.
- `sl2610_pek_cm52_can_functional_coverage_defconfig`
  Testcase: `CONFIG_APP_CAN_TEST_CASE_FUNCTIONAL_COVERAGE`
  Coverage: runtime configuration checks, Classical CAN DLC sweep, CAN FD payload/DLC matrix validation, RX accept-all, high-rate scheduled TX/RX burst validation, error-state checks, and scheduled TX timestamp validation.

Functional coverage options can be tuned in menuconfig:
- Classical CAN identifier type:
  - `CONFIG_APP_CAN_FUNCTIONAL_CLASSIC_FRAME_STANDARD`
  - `CONFIG_APP_CAN_FUNCTIONAL_CLASSIC_FRAME_EXTENDED`
- CAN FD identifier type:
  - `CONFIG_APP_CAN_FUNCTIONAL_FD_FRAME_STANDARD`
  - `CONFIG_APP_CAN_FUNCTIONAL_FD_FRAME_EXTENDED`

## Logger Interface Configuration

The checked-in CAN defconfigs enable the UART0 logger:
- `CONFIG_LOGGER_IF_UART_0=y`

Observe logs on the UART0 console unless you intentionally change the logger configuration.

## Connections

### General host connections

- Power the board using the designated power adapter.
- Connect the UART0 slot to the host machine using a UART cable for serial logs.
- Connect the USB2 slot to the host machine if you are using the USB image-loading workflow.

### Single-board validation

The loopback-oriented presets run through internal controller loopback and do not require external CAN wiring:
- `sl2610_pek_cm52_can_classic_loopback_defconfig`
- `sl2610_pek_cm52_can_fd_loopback_defconfig`
- `sl2610_pek_cm52_can_classic_rtr_loopback_defconfig`
- `sl2610_pek_cm52_can_scheduled_tx_defconfig`
- `sl2610_pek_cm52_can_rx_accept_all_defconfig`

### External dual-instance validation

The dual-instance and full functional-coverage presets require the PEK CAN0/CAN1 external route to be available:
- `sl2610_pek_cm52_can_dual_instance_defconfig`
- `sl2610_pek_cm52_can_functional_coverage_defconfig`

Use these presets only when the board setup connects the CAN0 transmit/receive path to CAN1 and both boards/signals share a common ground as required by the setup.

## Building and Flashing the Example using VS Code

Use the VS Code flow described in the respective soc vscode guides and the VS Code Extension guide:
- [SL2610 Build and Flash with VS Code](../../../docs/SL2610/SL2610_Build_and_Flash_with_VSCode.md)
- [Astra MCU SDK VS Code Extension User Guide](../../../docs/Astra_MCU_SDK_VSCode_Extension_User_Guide.md)

**Build (VS Code):**
1. Open **Build and Deploy** -> **Build Configurations**.
2. Select the **can_sample_app** project configuration in the **Project Configuration** dropdown.
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
   cd <sdk-root>/examples/driver_examples/can_sample_app
   export SRSDK_DIR=<sdk-root>
   make clean
   make <app_defconfig> BUILD=SRSDK
   ```
2. For faster rebuilds when only app code changes, reuse the app-local installed SDK package:
   ```bash
   cd <sdk-root>/examples/driver_examples/can_sample_app
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
   cd <sdk-root>/examples/driver_examples/can_sample_app
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
3. CAN sample logs appear in the logger window, including testcase selection, controller configuration, TX/RX status, timestamp checks, and final pass/fail status.

### Sample applications logs and output

1. **Classical CAN Loopback Application**
   - Console logs.
   ```bash
   Starting CAN Sample App!
   CAN testcase: LOOPBACK_CLASSIC
   CAN0 config: cl=500000 fd=2000000 ...
   CAN sample completed successfully
   ```

2. **CAN FD Loopback Application**
   - Console logs.
   ```bash
   Starting CAN Sample App!
   CAN testcase: LOOPBACK_FD
   CAN0 config: cl=500000 fd=2000000 ...
   CAN FD payload/DLC matrix validation completed successfully
   ```

3. **Dual Instance Application**
   - Console logs.
   ```bash
   Starting CAN Sample App!
   CAN testcase: DUAL_INSTANCE_TX_RX
   CAN0->CAN1 transfer validation completed successfully
   CAN sample completed successfully
   ```
