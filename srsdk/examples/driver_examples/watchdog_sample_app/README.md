# Watchdog Driver Sample Application

## Description

The Watchdog Driver sample application demonstrates watchdog start, refresh, stop, restart, and timeout-reset behavior on the supported boards for this application. It performs end-to-end watchdog functional validation with both reset and non-reset scenarios.

The sample includes five watchdog verification test cases:
- **WDG-FN-01:** Start watchdog and do not service it (system reset expected).
- **WDG-FN-02:** 5s timeout with 2s periodic refresh for 10s (no reset expected).
- **WDG-FN-03:** 3s timeout with 1s periodic refresh for 10s (normal operation).
- **WDG-FN-04:** Stop and restart watchdog with a new timeout configuration.
- **WDG-FN-05:** Multiple refreshes before expiry with 2s timeout and 0.5s refresh interval.

During each run, the app logs initialization status, testcase progression, and pass/fail results. For `WDG-FN-01`, the first boot intentionally triggers a watchdog reset and the next boot continues with post-reset verification plus remaining testcases.

The latest example structure uses a **common application source tree** with board-specific hardware setup kept under `hw/<BOARD>/`. For this app:
- Common application sources such as `main.c`, `watchdog_sample_app.c`, and `watchdog_sample_app.h` stay in the app root.
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

## Test Case Selection

Before building, choose the testcase defconfig that matches your target board.

You can:
- Select the required defconfig directly from the application's `configs/` directory.
- Run `make list_defconfigs` from the application directory to list all supported defconfigs.

**Available watchdog defconfigs (SL2610 RDK):**
- `sl2610_rdk_cm52_watchdog_sample_app_defconfig`: Normal watchdog path (`CONFIG_APP_WATCHDOG_INSTANCE=0`).
- `sl2610_rdk_cm52_watchdog_sample_app_slowclk_defconfig`: Slowclk watchdog path (`CONFIG_APP_WATCHDOG_INSTANCE=3`).

## Building and Flashing the Example using VS Code

Use the VS Code flow described in the respective soc vscode guides and the VS Code Extension guide:
- [SL2610 Build and Flash with VS Code](../../../docs/SL2610/SL2610_Build_and_Flash_with_VSCode.md)
- [Astra MCU SDK VS Code Extension User Guide](../../../docs/Astra_MCU_SDK_VSCode_Extension_User_Guide.md)

**Build (VS Code):**
1. Open **Build and Deploy** -> **Build Configurations**.
2. Select the **watchdog_sample_app** project configuration in the **Project Configuration** dropdown.
3. Select the required watchdog defconfig.
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
   cd <sdk-root>/examples/driver_examples/watchdog_sample_app
   export SRSDK_DIR=<sdk-root>
   make <app_defconfig> BUILD=SRSDK
   ```
2. For faster rebuilds when only app code changes, reuse the app-local installed SDK package:
   ```bash
   cd <sdk-root>/examples/driver_examples/watchdog_sample_app
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
   cd <sdk-root>/examples/driver_examples/watchdog_sample_app
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
3. The watchdog sample logs appear in the logger window, including phase transition across reset and final pass/fail summary.

**Expected Logs**

**First boot (WDG-FN-01 Phase 1, reset expected):**
```
BL: MCU Init...
send command successfully
SW_STRAP:0x01
Init DDR4 8x2 @ 3200
DHL:v0p41 PT:v0p40 ID:0x21010000
Boot from eMMC Flash
SM image verified
0000000000:[0][INF][SYS ]:Application drivers initialization complete without errors.
0000000000:[0][INF][SYS ]:sl2610 SDK version 1.3.0
0000000000:[0][INF][WDT ]:========================================
0000000005:[0][INF][WDT ]:  Watchdog Sample Application (SL2610)
0000000011:[0][INF][WDT ]:========================================
0000000017:[0][INF][WDT ]:Running WDG-FN-01 (will cause system reset)...
0000000024:[0][INF][WDT ]:=== WDG-FN-01: WATCHDOG RESET TEST Phase 1 ===
0000000030:[0][INF][WDT ]:Starting watchdog with 2s timeout, no servicing.
0000000037:[0][INF][WDT ]:System should reset in ~2 seconds.
0000000042:[0][INF][WDT ]:NOTE: SL2610 has no AON - verify reset by observing reboot.

```
**WDG-FN-01 Phase 2 + remaining testcases:**
```
BL: MCU Init...
send command successfully
SW_STRAP:0x01
Init DDR4 8x2 @ 3200
DHL:v0p41 PT:v0p40 ID:0x21010000
Boot from eMMC Flash
SM image verified
0000000000:[0][INF][SYS ]:Application drivers initialization complete without errors.
0000000000:[0][INF][SYS ]:sl2610 SDK version 1.3.0
0000000000:[0][INF][WDT ]:========================================
0000000005:[0][INF][WDT ]:  Watchdog Sample Application (SL2610)
0000000011:[0][INF][WDT ]:========================================
0000000017:[0][INF][WDT ]:Detected post-reset phase for WDG-FN-01
0000000023:[0][INF][WDT ]:=== WDG-FN-01: WATCHDOG RESET TEST Phase 2 ===
0000000030:[0][INF][WDT ]:System reset occurred after watchdog timeout!
0000000036:[0][INF][WDT ]:PASS: WDG-FN-01 - System reset detected (no AON verification)
0000000044:[0][INF][WDT ]:=== WDG-FN-02: 5s timeout, 2s refresh, 10s duration ===
0000002051:[0][INF][WDT ]:[t=2s] Refreshing watchdog...
0000004056:[0][INF][WDT ]:[t=4s] Refreshing watchdog...
0000006061:[0][INF][WDT ]:[t=6s] Refreshing watchdog...
0000008066:[0][INF][WDT ]:[t=8s] Refreshing watchdog...
0000010071:[0][INF][WDT ]:[t=10s] Refreshing watchdog...
0000010076:[0][INF][WDT ]:PASS: WDG-FN-02 - No reset occurred
0000010082:[0][INF][WDT ]:=== WDG-FN-03: 3s timeout, 1s refresh, 10s duration ===
0000011089:[0][INF][WDT ]:[t=1s] Refreshing watchdog...
0000012094:[0][INF][WDT ]:[t=2s] Refreshing watchdog...
0000013099:[0][INF][WDT ]:[t=3s] Refreshing watchdog...
0000014104:[0][INF][WDT ]:[t=4s] Refreshing watchdog...
0000015109:[0][INF][WDT ]:[t=5s] Refreshing watchdog...
0000016114:[0][INF][WDT ]:[t=6s] Refreshing watchdog...
0000017119:[0][INF][WDT ]:[t=7s] Refreshing watchdog...
0000018124:[0][INF][WDT ]:[t=8s] Refreshing watchdog...
0000019129:[0][INF][WDT ]:[t=9s] Refreshing watchdog...
0000020134:[0][INF][WDT ]:[t=10s] Refreshing watchdog...
0000020139:[0][INF][WDT ]:PASS: WDG-FN-03 - Watchdog did not trigger during normal operation
0000020147:[0][INF][WDT ]:=== WDG-FN-04: Stop/restart with new timeout ===
0000020154:[0][INF][WDT ]:Starting with 5s timeout
0000020158:[0][INF][WDT ]:Waiting 2s then refreshing...
0000022163:[0][INF][WDT ]:Waiting 1s more (t=3s) then stopping...
0000023169:[0][INF][WDT ]:Restarting with 4s timeout
0000025174:[0][INF][WDT ]:PASS: WDG-FN-04 - Stop/restart with new timeout successful
0000025182:[0][INF][WDT ]:=== WDG-FN-05: 2s timeout, 0.5s refresh, 8 refreshes ===
0000025689:[0][INF][WDT ]:Refresh #1 OK
0000026193:[0][INF][WDT ]:Refresh #2 OK
0000026696:[0][INF][WDT ]:Refresh #3 OK
0000027200:[0][INF][WDT ]:Refresh #4 OK
0000027703:[0][INF][WDT ]:Refresh #5 OK
0000028207:[0][INF][WDT ]:Refresh #6 OK
0000028711:[0][INF][WDT ]:Refresh #7 OK
0000029214:[0][INF][WDT ]:Refresh #8 OK
0000029218:[0][INF][WDT ]:PASS: WDG-FN-05 - All 8 refreshes succeeded
0000029224:[0][INF][WDT ]:========================================
0000029230:[0][INF][WDT ]:  Results: 5 passed, 0 failed (of 5)
0000029236:[0][INF][WDT ]:========================================
0000029242:[0][INF][WDT ]:All watchdog tests complete!


```
