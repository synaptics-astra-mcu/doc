# I2S Driver Sample Application

## Description

The I2S Driver sample application demonstrates I2S audio data transfer on the supported boards for this application. It performs end-to-end I2S validation including transmit, receive, loopback, dual-board communication, and DMA-driven buffering.

The sample includes multiple I2S verification modes:
- **Single-board loopback:** Asynchronous TX with RX loopback on the same board.
- **Dual TX node:** Transmit-only node for dual-board testing.
- **Dual RX node:** Receive-only node with integrity validation.
- **Dual RX replay:** Capture on RX and replay the captured data on TX.

During each run, the app logs initialization status, configuration details, DMA rate/IRQ stats, and validation results. This makes it easy for end users to confirm that I2S setup and data flow are working as expected.

The latest example structure uses a **common application source tree** with board-specific hardware setup kept under `hw/<BOARD>/`. For this app:
- Common application sources such as `main.c`, `i2s_sample_app.c`, and `i2s_sample_app.h` stay in the app root.
- Application defconfigs are stored under `configs/`.
- Board and hardware-specific setup is selected from `hw/<BOARD>/`, for example `hw/SL2610_RDK/`.

The application can also be exported and built as a **standalone app repository**. In that flow, keep this app in its own directory, point `SRSDK_DIR` to the SDK root, and build from the app directory itself. For the full application workflow model, see [Astra MCU SDK User Guide](../../../docs/Astra_MCU_SDK_User_Guide.md).

## Supported Boards

This application supports:
- `SL2610_RDK`

Select the defconfig that matches your target board and the desired I2S mode, and the build system will pick the corresponding board-specific hardware setup from `hw/<BOARD>/`.

## Prerequisites
- Choose **one** setup path:
  - **CLI**: [Setup and Install SDK using CLI](../../../docs/Astra_MCU_SDK_Setup_and_Install_CLI.md)
  - **VS Code**: [Setup and Install SDK using VS Code](../../../docs/Astra_MCU_SDK_Setup_and_Install_VsCode.md)

## Test Case Selection

Before building, choose the testcase defconfig that matches both your target board and the I2S mode you want to validate.

You can:
- Select the required defconfig directly from the application's `configs/` directory.
- Run `make list_defconfigs` from the application directory to list all supported defconfigs.

**Available I2S defconfigs (SL2610 RDK):**
- `sl2610_rdk_cm52_i2s_sample_app_single_board_loopback_defconfig`: Async TX with RX loopback on a single board.
- `sl2610_rdk_cm52_i2s_sample_app_dual_tx_defconfig`: Dual-board TX node image.
- `sl2610_rdk_cm52_i2s_sample_app_dual_rx_defconfig`: Dual-board RX node image.
- `sl2610_rdk_cm52_i2s_sample_app_dual_rx_replay_defconfig`: Dual-board RX capture with TX replay.

For this app, the default defconfig is:
   - `sl2610_rdk_cm52_i2s_sample_app_single_board_loopback_defconfig`

## Building and Flashing the Example using VS Code

Use the VS Code flow described in the respective soc vscode guides and the VS Code Extension guide:
- [SL2610 Build and Flash with VS Code](../../../docs/SL2610/SL2610_Build_and_Flash_with_VSCode.md)
- [Astra MCU SDK VS Code Extension User Guide](../../../docs/Astra_MCU_SDK_VSCode_Extension_User_Guide.md)

**Build (VS Code):**
1. Open **Build and Deploy** -> **Build Configurations**.
2. Select **i2s_sample_app** in the **Application** dropdown.
3. Select the required defconfig for the I2S mode you want to validate.
4. Build with **Build (SDK + App)** for the first build, or **Build App** for rebuilds.

**Flash (VS Code):**
1. Use **Image Conversion** to generate the flash image.
2. Use **Image Flashing** (SWD/JTAG) to flash the firmware image.

---

## Building and Flashing the Example using CLI

Use the CLI flow described in the respective build guide:
- [SL2610 Build and Flash with CLI](../../../docs/SL2610/SL2610_Build_and_Flash_with_CLI.md)
- [Astra MCU SDK User Guide](../../../docs/Astra_MCU_SDK_User_Guide.md)

**Build (CLI):**
1. Build from the application directory itself:
   ```bash
   cd <sdk-root>/examples/driver_examples/i2s_sample_app
   export SRSDK_DIR=<sdk-root>
   make <app_defconfig> BUILD=SRSDK
   ```
2. For faster rebuilds when only app code changes, reuse the app-local installed SDK package:
   ```bash
   cd <sdk-root>/examples/driver_examples/i2s_sample_app
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
   cd <sdk-root>/examples/driver_examples/i2s_sample_app
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
3. For I2S signal observation, connect a logic analyzer to the I2S pins used by your board.
4. I2S sample logs appear in the logger window, including transfer progress and validation results.

**Expected Logs**

**Single board loopback:**
```
0000000000:[0][WRN][LOGR]:Changing logger interface to LOGGER_IF_UART_0
0000000000:[0][INF][SYS ]:M52:: Build Date 07-04-2026 Time 07:28:29 Commit 727d936b
0000000000:[0][INF][SYS ]:sl2610 SDK version 1.3.0
0000000000:[0][INF][I2S ]:I2S Sample App Starts
0000000004:[0][INF][I2S ]:Mode: SINGLE board ASYNC TX with RX loopback
0000000010:[0][DBG][I2S ]:[I2S][APP_TX] soc_addr=0xe1006854 bytes=3840
0000000017:[0][INF][I2S ]:[I2S][DHUB_RATE] start: period_frames=480 expected_irq/s=100 repeat=0
0000000225:[0][INF][I2S ]:loopback done: pass=10 fail=0 tx_buf_cb=20 rx_buf_cb=10
0000000232:[0][INF][I2S ]:I2S Sample App End

```
**Dual TX:**
```
0000000000:[0][WRN][LOGR]:Changing logger interface to LOGGER_IF_UART_0
0000000000:[0][INF][SYS ]:M52:: Build Date 07-04-2026 Time 07:35:38 Commit 727d936b
0000000000:[0][INF][SYS ]:sl2610 SDK version 1.3.0
0000000000:[0][INF][I2S ]:I2S Sample App Starts
0000000004:[0][INF][I2S ]:Mode: DUAL board TX node
0000000008:[0][DBG][I2S ]:[I2S][APP_TX] soc_addr=0xe1006854 bytes=3840
0000000015:[0][INF][I2S ]:[I2S][DHUB_RATE] start: period_frames=480 expected_irq/s=100 repeat=0
0000001024:[0][INF][I2S ]:[I2S][DHUB_RATE] cmd/s=101 irq/s=100 irq_per_cmd=0.990 window_ms=1009 expected_irq/s=100
0000002027:[0][INF][I2S ]:[I2S][DHUB_RATE] cmd/s=100 irq/s=100 irq_per_cmd=1.000 window_ms=1003 expected_irq/s=100
0000003030:[0][INF][I2S ]:[I2S][DHUB_RATE] cmd/s=100 irq/s=100 irq_per_cmd=1.000 window_ms=1003 expected_irq/s=100
0000004032:[0][INF][I2S ]:[I2S][DHUB_RATE] cmd/s=100 irq/s=100 irq_per_cmd=1.000 window_ms=1002 expected_irq/s=100
0000006023:[0][INF][I2S ]:DUAL board TX complete, tx_complete_irq_count=481
0000006029:[0][INF][I2S ]:I2S Sample App End

```

**DUAL RX:**
```
0000000000:[0][WRN][LOGR]:Changing logger interface to LOGGER_IF_UART_0
0000000000:[0][INF][SYS ]:M52:: Build Date 07-04-2026 Time 07:44:51 Commit 727d936b
0000000000:[0][INF][SYS ]:sl2610 SDK version 1.3.0
0000000000:[0][INF][I2S ]:I2S Sample App Starts
0000000004:[0][INF][I2S ]:Mode: DUAL board RX node
0000000028:[0][INF][I2S ]:DUAL board RX integrity PASS (start=0 offset=35)
0000000034:[0][INF][I2S ]:I2S Sample App End

```
**DUAL RX Replay:**
```
0000000000:[0][WRN][LOGR]:Changing logger interface to LOGGER_IF_UART_0
0000000000:[0][INF][SYS ]:M52:: Build Date 07-04-2026 Time 07:38:53 Commit 727d936b
0000000000:[0][INF][SYS ]:sl2610 SDK version 1.3.0
0000000000:[0][INF][I2S ]:I2S Sample App Starts
0000000004:[0][INF][I2S ]:Mode: DUAL board RX node + TX replay (all IRQ)
0000000012:[0][INF][I2S ]:Starting RX capture for replay...
0000020987:[0][INF][I2S ]:Connect it with DAC
0000050991:[0][INF][GENR]:Starting TX replay...
0000050995:[0][INF][I2S ]:TX replay block_size=3840 bytes (pattern=3840 bytes)
0000051002:[0][DBG][I2S ]:[I2S][APP_TX] soc_addr=0xe1007754 bytes=3840
0000051008:[0][INF][I2S ]:[I2S][DHUB_RATE] start: period_frames=480 expected_irq/s=100 repeat=0
0000051017:[0][INF][I2S ]:Post start: TX state=2 tx_irq=1 rx_irq=1 err=0 waited=0msDUAL RX replay TX complete
0000051026:[0][INF][I2S ]:I2S Sample App End

```
