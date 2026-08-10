# SR110 Build and Flash with CLI

This document provides concise, CLI-only steps to build, convert, and flash SR110 applications. For additional SDK details, see [Astra MCU SDK User Guide](../Astra_MCU_SDK_User_Guide.md).

Throughout this guide, `<sdk-root>` refers to the directory where you extracted or cloned the SDK.

## Table of Contents
- [Prerequisites](#prerequisites)
- [CLI Flow (Build + Image Conversion + Flash)](#cli-flow-build--image-conversion--flash)
- [Running Examples](#running-examples)

## Prerequisites

- Astra Machina Micro connected per the [SR110 Platform Guide](./SR110_platform_Guide.md).
- CLI environment, toolchains, Python tools, and OpenOCD installed. See [Setup and Install SDK using CLI](../Astra_MCU_SDK_Setup_and_Install_CLI.md).
- Python virtual environment activated. See [Setup and Install SDK using CLI](../Astra_MCU_SDK_Setup_and_Install_CLI.md).

## CLI Flow (Build + Image Conversion + Flash)

1. Build the application (SDK build + example build):

   List all available application defconfigs (these are the presets you can build):
   ```bash
   ls <sdk-root>/examples/<example_type>/<app>/configs/*defconfig
   ```

   Recommended first example: `sr110_rdk_cm55_demo_sample_app_defconfig`.

   Set `SRSDK_DIR` to `<sdk-root>` and build (run from `<sdk-root>/examples/<example_type>/<app>`).

   ```bash
   cd <sdk-root>/examples/<example_type>/<app>
   export SRSDK_DIR=<sdk-root>

   make sr110_rdk_cm55_demo_sample_app_defconfig BUILD=SRSDK
   ```

   Expected output:
   ```
   <sdk-root>/examples/<example_type>/<app>/out/sr110_cm55_fw/release/sr110_cm55_fw.elf   # GCC/LLVM
   <sdk-root>/examples/<example_type>/<app>/out/sr110_cm55_fw/release/sr110_cm55_fw.axf   # AC6
   ```

2. Generate the flash image (SDK image generator; run from `<sdk-root>/tools/srsdk_image_generator`):

   ```bash
   cd <sdk-root>/tools/srsdk_image_generator
   python srsdk_image_generator.py \
     -B0 \
     -flash_image \
     -sdk_secured \
     -spk "<sdk-root>/tools/srsdk_image_generator/Inputs/spk_rc4_1_0_secure_otpk.bin" \
     -apbl "<sdk-root>/tools/srsdk_image_generator/Inputs/sr100_b0_bootloader_ver_0x012F_ASIC.axf" \
     -m55_image "<sdk-root>/examples/<example_type>/<app>/out/sr110_cm55_fw/release/sr110_cm55_fw.elf" \
     -flash_type "GD25LE128" \
     -flash_freq "67"
   ```

   Expected output:
   ```
   <sdk-root>/tools/srsdk_image_generator/Output/B0_Flash/B0_flash_full_image_GD25LE128_67Mhz_secured.bin
   ```

3. Flash the image to external flash, ensure J14 is plugged in.

   If you are running WSL, please consult the [Astra MCU SDK - WSL User Guide](../Astra_MCU_SDK_WSL_User_Guide.md) to ensure USB ports are properly handled.

   ```bash
   cd <sdk-root>
   python tools/openocd/scripts/flash_xspi_tcl.py \
     --cfg_path tools/openocd/configs/sr110_m55.cfg \
     --image tools/srsdk_image_generator/Output/B0_Flash/B0_flash_full_image_GD25LE128_67Mhz_secured.bin \
     --erase-all
   ```

   - Use `--probe jlink` if you are flashing with J-Link.
   - Expected result: the flash tool completes without errors and the image is programmed.

4. Reset the board to boot from external flash:
   - Unplug and replug USB, or press the reset button.

## Running Examples

After flashing, reset the board and follow the [README](../../../user_guide/index.rst) for the example you built.
