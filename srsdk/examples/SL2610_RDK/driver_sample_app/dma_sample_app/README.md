# DMA Sample Application

## Description

This DMA sample application demonstrates advanced DMA usage including 1D memory transfers, linked descriptor chains, and software-triggered transfers. It highlights DMA channel management, pause/resume control, interrupt-driven completion handling, cache maintenance, and data integrity validation under FreeRTOS.

## Prerequisites
- Choose **one** setup path:
  - **CLI**: [Setup and Install SDK using CLI](../../../../docs/Astra_MCU_SDK_Setup_and_Install_CLI.md)
  - **VS Code**: [Setup and Install SDK using VS Code](../../../../docs/Astra_MCU_SDK_Setup_and_Install_VsCode.md)

## Building and Flashing the Example using VS Code

Use the VS Code flow described in the SL2610 guides and VS Code Extension guide:
- [SL2610 Platform Guide](../../../../docs/SL2610/SL2610_Platform_Guide.md)
- [Astra MCU SDK VS Code Extension User Guide](../../../../docs/Astra_MCU_SDK_VSCode_Extension_User_Guide.md)

**Build (VS Code):**
1. Open **Build and Deploy** -> **Build Configurations**.
2. Select `cm52_dma_sample_app_rdk` in the **Application** dropdown.
3. Build with **Build (SDK + App)** for the first build, or **Build App** for rebuilds.

![Build Configurations](assets/dma_sample_app_sl2610.png)

**Flash/Image Generation (VS Code):**
1. Build the SL2610 bootloader image.
2. Generate output binaries (equivalent of `make imagegen`) and collect:
   - `sl2610_bootloader_extras.bin`
   - `sl2610_bootloader_output.bin`
   - `sl2610_cm52_fw_extras.bin`
   - `sl2610_cm52_fw_output.bin`
3. Copy generated binaries to VSSDK.
4. Generate the system sub-image in VSSDK.
   - Refer: [SL2610 Platform Guide - Image Generation](../../../../docs/SL2610/SL2610_Platform_Guide.md#image-generation-2)
5. Flash/download the MCU image to target.
   - Refer: [SL2610 Platform Guide - Image Flashing](../../../../docs/SL2610/SL2610_Platform_Guide.md#image-flashing)

---

## Building and Flashing the Example using CLI

Use the CLI flow described in the SL2610 Platform Guide:
- [SL2610 Platform Guide](../../../../docs/SL2610/SL2610_Platform_Guide.md)

**Build (CLI):**
1. Build the SL2610 bootloader from SDK root:
   ```bash
   make sl2610_bootloader_rdk_defconfig BOARD=SL2610_RDK
   make
   ```
2. Build DMA sample app from `<sdk-root>/examples`:
   ```bash
   cd <sdk-root>/examples
   export SRSDK_DIR=<sdk-root>
   make cm52_dma_sample_app_rdk_defconfig BOARD=SL2610_RDK BUILD=SRSDK
   ```

**Image Generation and Flash (CLI):**
> **Note:** SL2610 image generation is not supported on native Windows.
> Use WSL for image generation.
> In WSL, ensure required tools are installed: Python, `make`, and Arm GNU toolchain.
> You can use the VS Code extension's Tools Installer in WSL, or follow
> [Linux Environment guide](../../../../docs/build_env/Astra_MCU_SDK_Linux_env_with_gcc.md) for CLI setup.

1. Generate binaries:
   ```bash
   cd <sdk-root>/examples
   export SRSDK_DIR=<sdk-root>
   make imagegen
   ```
2. Copy generated binaries to VSSDK:
   - `sl2610_bootloader_extras.bin`
   - `sl2610_bootloader_output.bin`
   - `sl2610_cm52_fw_extras.bin`
   - `sl2610_cm52_fw_output.bin`
3. Generate system sub-image in VSSDK.
   - Refer: [SL2610 Platform Guide - Image Generation](../../../../docs/SL2610/SL2610_Platform_Guide.md#image-generation-2)
4. Flash/download image to target.
   - Refer: [SL2610 Platform Guide - Image Flashing](../../../../docs/SL2610/SL2610_Platform_Guide.md#image-flashing)

---

## Running the Application using VS Code Extension

1. Power the board and press **RESET** after flashing.
2. For logging output, connect UART to the target and open a serial console (for example, MobaXterm).
3. DMA sample logs appear in the serial window.

**Expected Logs**

```
0000000000:[0][WRN][LOGR]:Changing logger interface to LOGGER_IF_UART_0
0000000000:[0][INF][SYS ]:M52:: Build Date 24-02-2026 Time 15:46:06 Commit 9d732429
0000000000:[0][INF][SYS ]:sl2610 SDK version 1.3.0
0000000000:[0][INF][DMA ]:Starting DMA Sample Application...
0000000005:[0][INF][DMA ]:Running All DMA Transfer Tests...
0000000010:[0][INF][DMA ]:Channel 0 allocated successfully.
0000000016:[0][INF][DMA ]:[DMA] Preparing 1D transfer: 64 elements from 30001390 to 30001490
0000000024:[0][INF][DMA ]:Transfer Paused.
0000000028:[0][INF][DMA ]:Transfer resumed.
0000000032:[0][INF][DMA ]:[DMA] 1D transfer successful: 64 elements verified
0000000038:[0][INF][DMA ]:Transfer Paused.
0000000042:[0][INF][DMA ]:Transfer resumed.
0000000046:[0][INF][DMA ]:Linked transfers verified successfully.
0000000052:[0][INF][DMA ]:Channel 0 allocated successfully.
0000000057:[0][INF][DMA ]:Final status = 0x10000
0000000062:[0][INF][DMA ]:Transfer completed. Verifying data...
0000000068:[0][INF][DMA ]:SW-triggered DMA transfer successful!
0000000073:[0][INF][DMA ]:All DMA transfers successful.
0000000078:[0][INF][DMA ]:DMA Sample Application Completed.
```
