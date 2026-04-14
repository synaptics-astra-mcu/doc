# SL2610 Build and Flash with CLI

This document provides concise, CLI-only steps to build and flash SL2610 applications.

Throughout this guide, `<sdk-root>` refers to the directory where you extracted or cloned the SDK.

## Table of Contents
- [Prerequisites](#prerequisites)
- [CLI Flow (Build + Image Generation + Flash)](#cli-flow-build--image-generation--flash)
- [WSL USB Flashing Flow (usbipd)](#wsl-usb-flashing-flow-usbipd)
- [Notes](#notes)

## Prerequisites

- SL2610 RDK connected with 5V USB-C power (PWR_IN) and USB 2.0 OTG to the host.
- Ensure hardware connections are set up per the [SL2610 Platform Guide](./SL2610_Platform_Guide.md).
- CLI environment, toolchains, and Python tools installed. See [Setup and Install SDK using CLI](../Astra_MCU_SDK_Setup_and_Install_CLI.md).
- Python virtual environment activated. See [Setup and Install SDK using CLI](../Astra_MCU_SDK_Setup_and_Install_CLI.md).

## CLI Flow (Build + Image Generation + Flash)

1. Build the System Manager and bootloader (SDK build + image assets):

   ```bash
   cd <sdk-root>/examples/system_manager
   export SRSDK_DIR=<sdk-root>
   make sl2610_rdk_system_manager_defconfig BUILD=SRSDK

   cd <sdk-root>
   make sl2610_bootloader_rdk_defconfig BOARD=SL2610_RDK
   make astrasdk
   ```

   - The first command builds the System Manager using the SDK build system.
   - The second pair builds the bootloader needed for USB boot and image packaging.

2. Generate MCU sub-images using the SDK image generator (run from `<sdk-root>/examples/<example_type>/<app>`):

   Before running `make imagegen`, ensure the bootloader is built:
   ```bash
   cd <sdk-root>
   make sl2610_bootloader_rdk_defconfig BOARD=SL2610_RDK
   make astrasdk
   ```

   ```bash
   cd <sdk-root>/examples/<example_type>/<app>
   make imagegen
   ```

   Expected outputs:
   - System Manager sub-image (for `run-sm`, uncompressed): `<sdk-root>/examples/<example_type>/<app>/out/image/eMMCimg/sysmgr.subimg.gz`
   - Compressed sub-images (for eMMC packaging, includes `sysmgr.subimg.gz`): `<sdk-root>/examples/<example_type>/<app>/out/image/eMMCimg/`
   - USB boot inputs (SPK/keys/bootloader): `<sdk-root>/examples/<example_type>/<app>/out/image/usb_boot/`

   If you see permission errors on Linux/macOS, run:
   ```bash
   cd <sdk-root>/examples
   chmod +x tools/scripts/image/bin/gen*
   ```

3. Flash with the USB boot tool (SDK USB boot utility):

   **Enter USB boot mode first:**
   - Press and hold **USB_BOOT**, then press **RESET**.
   - Release **RESET**, then release **USB_BOOT**.
   - There are two supported modes of flashing
      - Flash Entire eMMC Folder
      - Flash System Manager Sub-Image


   **Flash Entire eMMC Folder**
   ```bash
   cd <sdk-root>/tools/usb_boot_python_tool/USB_BOOT_TOOL
   python usb_boot_tool.py --op run-sm \
     --sm <sdk-root>/examples/<example_type>/<app>/out/image/eMMCimg/sysmgr.subimg.gz \
     --spk <sdk-root>/examples/<example_type>/<app>/out/image/usb_boot/spk.bin \
     --keys <sdk-root>/examples/<example_type>/<app>/out/image/usb_boot/key.bin \
     --m52bl <sdk-root>/examples/<example_type>/<app>/out/image/usb_boot/m52bl.bin

   # For full eMMC flashing, use a Yocto-generated eMMCimg folder (see Notes).
   python usb_boot_tool.py --op emmc --img-dir <path-to-eMMCimg>
   ```

   **Flash System Manager Sub-Image**
   ```bash
   python usb_boot_tool.py --op emmc-sm \
     --sm <sdk-root>/examples/<example_type>/<app>/out/image/eMMCimg/sysmgr.subimg.gz \
     --sm-image <sdk-root>/examples/<example_type>/<app>/out/image/eMMCimg/sysmgr.subimg.gz
   ```

## WSL USB Flashing Flow (usbipd)

Use this flow when flashing from WSL (Windows Subsystem for Linux). It uses `usbipd` to forward SL2610 USB CDC interfaces from Windows into WSL.

### CDC VID:PID Reference

| Stage | VID | PID | Description |
|---|---|---|---|
| Initial Boot & BL Flash | `0x06CB` | `0x019E` | Device in bootloader mode (used for SPK, BL, keys upload) |
| After SM Flash | `0xCAFE` | `0x4002` | System Manager (SM) is running |

### Workflow Overview

```
[Device in BL mode]
       │
       ▼
 Step 1: Bind & Attach 0x06CB:0x019E  (Admin Windows Terminal)
       │
       ▼
 Step 2: Flash SPK  →  python usb_boot_tool.py --op run-spk
       │
       ▼
 Step 3: Device re-enumerates as 0x06CB:0x019E (M52BL CDC)
         Bind & Attach again
       │
       ▼
 Step 4: Flash SM   →  python usb_boot_tool.py --op run-sm --sm <path>
       │
       ▼
 Step 5: Device re-enumerates as 0xCAFE:0x4002 (SM CDC)
         Bind & Attach
       │
       ▼
 Step 6: Flash eMMC →  python usb_boot_tool.py --op emmc --img-dir <path>
```

### Flashing steps (WSL)

1. Identify the USB device (BL Flash CDC) in an Administrator Windows terminal:

   ```powershell
   usbipd list
   ```

   Find VID:PID `06CB:019E` and note its `BUSID` (for example `2-4`).

2. Bind and attach the BL CDC:

   ```powershell
   # Admin Windows terminal
   usbipd bind --busid <bus_id> --force

   # Second Windows terminal (non-admin)
   usbipd attach --wsl --busid <bus_id>
   ```

3. In WSL, verify USB and flash SPK:

   ```bash
   lsusb | grep "06cb"
   cd <sdk-root>/tools/usb_boot_python_tool/USB_BOOT_TOOL
   python usb_boot_tool.py --op run-spk
   ```

4. Re-bind and re-attach after re-enumeration (still `06CB:019E`):

   ```powershell
   # Admin Windows terminal
   usbipd bind --busid <bus_id> --force

   # Second Windows terminal (non-admin)
   usbipd attach --wsl --busid <bus_id>
   ```

   If `BUSID` changed after re-enumeration, run `usbipd list` again and use the new value.

5. In WSL, flash System Manager:

   ```bash
   python usb_boot_tool.py --op run-sm --sm <sdk-root>/examples/<example_type>/<app>/out/image/eMMCimg/sysmgr.subimg.gz
   ```

6. Bind and attach SM CDC (`CAFE:4002`):

   ```powershell
   # Admin Windows terminal
   usbipd list
   usbipd bind --busid <new_bus_id> --force

   # Second Windows terminal (non-admin)
   usbipd attach --wsl --busid <new_bus_id>
   ```

7. In WSL, flash eMMC:

   ```bash
   python usb_boot_tool.py --op emmc --img-dir <path-to-eMMCimg>
   ```

   Tip: If flashing does not start after running a command, unplug and replug the USB cable, then re-run `usbipd list`/`bind`/`attach` and retry.

### Quick Reference (WSL)

```powershell
# [Admin Windows terminal]
usbipd list
usbipd bind --busid <bus_id> --force
```

```powershell
# [Second Windows terminal]
usbipd attach --wsl --busid <bus_id>
```

```bash
# [WSL terminal]
cd <sdk-root>/tools/usb_boot_python_tool/USB_BOOT_TOOL
python usb_boot_tool.py --op run-spk
python usb_boot_tool.py --op run-sm --sm <sdk-root>/examples/<example_type>/<app>/out/image/eMMCimg/sysmgr.subimg.gz
python usb_boot_tool.py --op emmc --img-dir <path-to-eMMCimg>
```

## Notes

- Use `--op run-sm` to program only the System Manager sub-image.
- `--op emmc` requires an `eMMCimg` folder that includes `emmc_part_list` and `emmc_image_list`. These are generated by Yocto when you build the full Astra image.
- For SM-to-eMMC flashing (`--op emmc-sm`), ensure `emmc_image_list` is present in the `eMMCimg` folder.
- Yocto is only required if you need a full Linux/eMMC image; MCU image generation is handled entirely by the SDK tools above.
- If `run-sm` fails because SM CDC is already running, power-cycle the board and try again.
- `usbipd bind` must be run in an Administrator Windows terminal; `usbipd attach` can be run in a normal Windows terminal.
- After flashing from WSL, unbind/detach USB forwarding in Windows if you want normal native Windows USB access.
