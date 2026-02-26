# System Manager Application

## Description

The System Manager application for `SL2610_RDK` initializes core platform services and manages cold-boot flow selection. Based on the detected boot source, it routes boot handling to XSPI, eMMC, or USB boot paths and prepares the system for A55 boot handoff. It also supports USB boot services such as eMMC flashing and running A-core images when USB boot source is selected. In addition, it initializes core drivers/services (UART, timer, XSPI, logger, and system manager task) and provides runtime logs for bring-up and verification.

## Prerequisites
- Choose **one** setup path:
  - **CLI**: [Setup and Install SDK using CLI](../../../docs/Astra_MCU_SDK_Setup_and_Install_CLI.md)
  - **VS Code**: [Setup and Install SDK using VS Code](../../../docs/Astra_MCU_SDK_Setup_and_Install_VsCode.md)

## Building and Flashing the Example using VS Code

Use the VS Code flow described in the SL2610 guides and VS Code Extension guide:
- [SL2610 Platform Guide](../../../docs/SL2610/SL2610_Platform_Guide.md)
- [Astra MCU SDK VS Code Extension User Guide](../../../docs/Astra_MCU_SDK_VSCode_Extension_User_Guide.md)

**Build (VS Code):**
1. Open **Build and Deploy** -> **Build Configurations**.
2. Select one system manager configuration in the **Application** dropdown:
   - `sl2610_system_manager_rdk`
   - `sl2610_system_manager_ddr3_rdk`
   - `sl2610_system_manager_lpddr4_rdk`
3. Build with **Build (SDK + App)** for the first build, or **Build App** for rebuilds.

**Flash/Image Generation (VS Code):**
1. Build the SL2610 bootloader image.
2. Generate output binaries (equivalent of `make imagegen`) and collect:
   - `sl2610_bootloader_extras.bin`
   - `sl2610_bootloader_output.bin`
   - `sl2610_cm52_fw_extras.bin`
   - `sl2610_cm52_fw_output.bin`
3. Copy generated binaries to VSSDK.
4. Generate the system sub-image in VSSDK.
   - Refer: [SL2610 Platform Guide - Image Generation](../../../docs/SL2610/SL2610_Platform_Guide.md#image-generation-2)
5. Flash/download the MCU image to target.
   - Refer: [SL2610 Platform Guide - Image Flashing](../../../docs/SL2610/SL2610_Platform_Guide.md#image-flashing)

---

## Building and Flashing the Example using CLI

Use the CLI flow described in the SL2610 Platform Guide:
- [SL2610 Platform Guide](../../../docs/SL2610/SL2610_Platform_Guide.md)

**Build (CLI):**
1. Build the SL2610 bootloader from SDK root:
   ```bash
   make sl2610_bootloader_rdk_defconfig BOARD=SL2610_RDK
   make
   ```
2. Build one system manager target from `<sdk-root>/examples`:
   ```bash
   cd <sdk-root>/examples
   export SRSDK_DIR=<sdk-root>

   # Choose one:
   make sl2610_system_manager_rdk_defconfig BOARD=SL2610_RDK BUILD=SRSDK
   # or
   make sl2610_system_manager_ddr3_rdk_defconfig BOARD=SL2610_RDK BUILD=SRSDK
   # or
   make sl2610_system_manager_lpddr4_rdk_defconfig BOARD=SL2610_RDK BUILD=SRSDK
   ```

**Image Generation and Flash (CLI):**
> **Note:** SL2610 image generation is not supported on native Windows.
> Use WSL for image generation.
> In WSL, ensure required tools are installed: Python, `make`, and Arm GNU toolchain.
> You can use the VS Code extension's Tools Installer in WSL, or follow
> [Linux Environment guide](../../../docs/build_env/Astra_MCU_SDK_Linux_env_with_gcc.md) for CLI setup.

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
   - Refer: [SL2610 Platform Guide - Image Generation](../../../docs/SL2610/SL2610_Platform_Guide.md#image-generation-2)
4. Flash/download image to target.
   - Refer: [SL2610 Platform Guide - Image Flashing](../../../docs/SL2610/SL2610_Platform_Guide.md#image-flashing)

---

## Running the Application using VS Code Extension

1. Power the board and press **RESET** after flashing.
2. For logging output, click **SERIAL MONITOR** and connect to the **DAP logger** port on J14.
   - To make it easier to identify, ensure **only J14** is plugged in (not J13).
   - The logger port is not guaranteed to be consistent across OSes. As a starting point:
     - **Windows:** try the lower-numbered J14 COM port first.
     - **Linux/macOS:** try the higher-numbered J14 port first.
   - If you do not see logs after a reset, switch to the other J14 port.
3. System manager logs appear in the logger window.

**Expected Logs**

```
before lock sts: 0x20

 LCS OTP: 0xaead0303
after lock sts: 0x21

synaprot a0 spk: release id 0xfc9b8b5, commit time Mon Oct 13 19:27:42 2025 -0700
synaprot a0 hal : release id 0xf24e4cf, commit time Mon Oct 13 01:32:40 2025 -0700
Cold Boot...
speed_up clock_freq...
conf flightcontrol...
fc: reset0_faultasso0_FAULT: 0xf80000f8
fc: reset0_faultasso1_FAULT: 0xe0000007
Waiting command...
read msg data is 0x5b
read msg data is 0x5a
read msg data is 0x33
read msg data is 0x4
read msg data is 0xd4
read msg data is 0x33
read msg data is 0x1
read msg data is 0x0
read msg data is 0x0
read msg data is 0x0
read msg data is 0x0
read msg data is 0x0
read msg data is 0x0
read msg data is 0x0
read msg data is 0x0
read msg data is 0x0
read msg data is 0x0
read msg data is 0x0
read msg data is 0x0
read msg data is 0x0
read msg data is 0x0
read msg data is 0x0
read msg data is 0x0
read msg data is 0x0
read msg data is 0x0
read msg data is 0x0
read msg data is 0x0
read msg data is 0x0
read msg data is 0x0
read msg data is 0x0
read msg data is 0x0
read msg data is 0x0
download bl image

u32_num_bytes is 0x133d4, img_dwnld_addr=0x30002000
opcode download bl image
verify bl
image_buf: 0x30002000
bl header type: 0x00000011
image dest addr: 0x30020000, body_size: 0x00013160
clear BL !
save m52 entry: 0x30020000
save magic: 0x574d424c

 send rx msg: 0x5b 0x5a 0x33 0x4 0x0 0x0 0x0 0x0
relocate vector table...
Vector table is 128-byte aligned.
release M52...
BL: MCU Init...command...
Init DDR4 @ 3200
DHL:v0p40 PT:v0p40 ID:0x21010000
USB MOUNTED
0000000000:[0][WRN][LOGR]:Changing logger interface to LOGGER_IF_UART_0
0000000000:[0][INF][SYS ]:------------------------------------------
0000000000:[0][INF][SYS ]:            Hello  ASTRA
0000000000:[0][INF][SYS ]:------------------------------------------
0000000000:[0][INF][SYS ]:System initialization done
0000000000:[0][INF][SYS ]:M52:: Build Date 19-01-2026 Time 15:57:15 Commit unknown
0000000000:[0][INF][SYS ]:sl2610 SDK version 1.3.0
0000000000:[0][INF][USB ]:USB device unmounted
0000000004:[0][INF][HAPI]:Active interface is USB
0000000110:[0][INF][SYS ]:VCORE 800 mV
0000000322:[0][INF][USB ]:USB device mounted
0000001928:[0][INF][HAPI]:Requested Service ID: 13 and Opcode: 15
0000002055:[0][INF][HAPI]:Requested Service ID: 13 and Opcode: 15
0000002158:[0][INF][HAPI]:Requested Service ID: 13 and Opcode: 18
0000002196:[0][INF][HAPI]:Requested Service ID: 13 and Opcode: 15
0000002297:[0][INF][HAPI]:Requested Service ID: 13 and Opcode: 15
0000002407:[0][INF][HAPI]:Requested Service ID: 13 and Opcode: 15
0000000000:[0][WRN][LOGR]:Changing logger interface to LOGGER_IF_UART_0
0000000000:[0][INF][SYS ]:M52:: Build Date 22-01-2026 Time 18:02:38 Commit unknown
0000000000:[0][INF][SYS ]:sl2610 SDK version 1.3.0
0000000102:[0][INF][SYS ]:VCORE 800 mV
0000000106:[0][INF][SYS ]:eMMC: Boot A55
0000000176:[0][INF][SYS ]:Loading bl_a
0000000269:[0][INF][SYS ]:Loading tzk_a
NOTICE:  BL31: v2.7(release):v2.7.0-18-g483e9cedcf-dirty
NOTICE:  BL31: Built : 15:02:46, Jun  1 2022
```

After the above sequence, the System Manager hands off control and boots the A-core (A55).
