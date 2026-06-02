# Timer Driver Sample Application

## Description

The Timer Driver sample application validates timer APIs and both timer modes introduced in the updated naming model:
- `TIMER_MODE_ONE_SHOT`
- `TIMER_MODE_PERIODIC`

The app executes functional testcases and prints pass/fail status on the logger console.

Implemented testcases:
- **TMR-FN-01**: Validate blocking APIs (`timer_wait_us`, `timer_wait_ticks`).
- **TMR-FN-02**: Validate one-shot mode (`timer_start_us(..., TIMER_MODE_ONE_SHOT)`) callback behavior.
- **TMR-FN-03**: Validate periodic mode (`timer_start_us(..., TIMER_MODE_PERIODIC)`) and stop behavior.
- **TMR-FN-04**: Validate system counter progression and remaining-time query APIs (`timer_get_system_count`, `timer_get_remaining_ticks`, `timer_get_remaining_us`).
- **TMR-FN-05**: Validate cancellation of a running one-shot timer via `timer_stop`.
- **TMR-FN-06**: Optional slowclk mode validation (one-shot + periodic). If slowclk timers are unavailable on the target, this testcase is reported as skipped.

During each run, the app logs initialization status, testcase progression, and pass/fail results. This makes it easy for end users to confirm that the timer setup and API operations are working as expected.

The latest example structure uses a **common application source tree** with board-specific hardware setup kept under `hw/<BOARD>/`. For this app:
- Common application sources such as `main.c`, `timer_sample_app.c`, and `timer_sample_app.h` stay in the app root.
- Application defconfigs are stored under `configs/`.
- Board and hardware-specific setup is selected from `hw/<BOARD>/`, for example `hw/SL2610_RDK/`.

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
- `sl2610_rdk_cm52_timer_sample_app_defconfig`
- `sr110_rdk_cm55_timer_sample_app_defconfig`

For this app, the default defconfig is:
   - SR110_RDK: `sr110_rdk_cm55_timer_sample_app_defconfig`
   - SL2610_RDK: `sl2610_rdk_cm52_timer_sample_app_defconfig`

## Building and Flashing the Example using VS Code

Use the VS Code flow described in the respective soc vscode guides and the VS Code Extension guide:
- [SR110 Build and Flash with VS Code](../../../docs/SR110/SR110_Build_and_Flash_with_VSCode.md)
- [SL2610 Build and Flash with VS Code](../../../docs/SL2610/SL2610_Build_and_Flash_with_VSCode.md)
- [Astra MCU SDK VS Code Extension User Guide](../../../docs/Astra_MCU_SDK_VSCode_Extension_User_Guide.md)

**Build (VS Code):**
1. Open **Build and Deploy** -> **Build Configurations**.
2. Select the **timer_sample_app** project configuration in the **Project Configuration** dropdown.
3. Select the required defconfig for the application.
4. Build with **Build (SDK+Project)** for the first build, or **Build (Project)** for rebuilds.

**Flash (VS Code):**
1. For `SR110`:
   - Use **Image Conversion** to generate the flash image.
   - Use **Image Flashing** with **SWD/JTAG** to flash the firmware image.
2. For `SL2610`:
   - Use the SL2610 image-generation flow to generate the required sub-image.
   - Open **Image Flashing (SL2610)**.
   - Select **Flash Target** as **M52 Image**.
   - In **Image Path**, browse to and select the generated sub-image file, such as `sysmgr.subimg.gz`.
   - Start the flashing operation to program the image to the target.

---

## Building and Flashing the Example using CLI

Use the CLI flow described in the respective build guide:
- [SR110 Build and Flash with CLI](../../../docs/SR110/SR110_Build_and_Flash_with_CLI.md)
- [SL2610 Build and Flash with CLI](../../../docs/SL2610/SL2610_Build_and_Flash_with_CLI.md)
- [Astra MCU SDK User Guide](../../../docs/Astra_MCU_SDK_User_Guide.md)

**Build (CLI):**
1. Build from the application directory itself:
   ```bash
   cd <sdk-root>/examples/driver_examples/timer_sample_app
   export SRSDK_DIR=<sdk-root>
   make <app_defconfig> BUILD=SRSDK
   ```
2. For faster rebuilds when only app code changes, reuse the app-local installed SDK package:
   ```bash
   cd <sdk-root>/examples/driver_examples/timer_sample_app
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
     -m55_image "<sdk-root>/examples/driver_examples/timer_sample_app/out/sr110_cm55_fw/release/sr110_cm55_fw.elf" \
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
   cd <sdk-root>/examples/driver_examples/timer_sample_app
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
3. Timer sample logs appear in the logger window, including each testcase flow and pass/fail status.

**Expected Logs**

On a successful run, logs show the configured testcase flow for the selected board. For example:

```text
=== [TMR-FN-01] Blocking Wait API Test ===
...
=== [TMR-FN-01] Blocking Wait API Test PASSED ===
```

## Notes

- Test timing can be tuned via Kconfig options in `kconfig`.
- The sample avoids using blocking timer waits while periodic mode is active, to prevent self-contention in constrained timer-instance configurations.
</content>
