# SR110 Build and Flash with CLI

This document provides concise, CLI-only steps to build, convert, and flash SR110 applications. For additional SDK details, see [Astra MCU SDK User Guide](../Astra_MCU_SDK_User_Guide.md).

Throughout this guide, `<sdk-root>` refers to the folder where you extracted or cloned the SDK.

## Table of Contents
- [Prerequisites](#prerequisites)
- [CLI Flow (Build + Image Conversion + Flash)](#cli-flow-build--image-conversion--flash)
- [Running Examples](#running-examples)

## Prerequisites

- SR110 Astra Machina Micro (SR110_RDK) connected and powered with J14 USB connected to the host.
- Ensure hardware connections are set up per the [SR110 Platform Guide](./SR110_platform_Guide.md).
- CLI environment, toolchains, Python tools, and OpenOCD installed. See [Setup and Install SDK using CLI](../Setup_and_Install_SDK_using_CLI.md).
- Python virtual environment activated. See [Setup and Install SDK using CLI](../Setup_and_Install_SDK_using_CLI.md).

## CLI Flow (Build + Image Conversion + Flash)

1. Build the application (SDK build + example build):

   List all available application defconfigs (these are the presets you can build):
   ```bash
   ls SR110_RDK/configs/*defconfig
   ```

   Recommended first example: `cm55_demo_sample_app_defconfig`.

   Set `SRSDK_DIR` and build (run from `<sdk-root>/examples`).
   - The first `make` applies the defconfig and builds the SDK package for SR110.
   - The second `make build` builds the example using the installed SDK package.
   ```bash
   cd <sdk-root>/examples
   export SRSDK_DIR=<sdk-root>

   make cm55_demo_sample_app_defconfig BOARD=SR110_RDK BUILD=SRSDK
   make build BOARD=SR110_RDK
   ```

   Expected output:
   ```
   <sdk-root>/examples/out/sr110_cm55_fw/release/sr110_cm55_fw.elf
   ```

2. Generate the flash image (SDK image generator; run from `<sdk-root>/tools/srsdk_image_generator`):

   Use the provided SPK/APBL files unless your board documentation specifies different inputs.

   ```bash
   cd <sdk-root>/tools/srsdk_image_generator
   python srsdk_image_generator.py \
     -B0 \
     -flash_image \
     -sdk_secured \
     -spk "<sdk-root>/tools/srsdk_image_generator/B0_Input_examples/spk_rc3_0_secure_otpk_0605.bin" \
     -apbl "<sdk-root>/tools/srsdk_image_generator/B0_Input_examples/sr100_b0_bootloader_ver_0x012F_ASIC.axf" \
     -m55_image "<sdk-root>/examples/out/sr110_cm55_fw/release/sr110_cm55_fw.elf" \
     -flash_type "GD25LE128" \
     -flash_freq "67"
   ```

   Expected output:
   ```
   <sdk-root>/examples/out/bin_files/Output/B0_Flash/B0_flash_full_image_GD25LE128_67Mhz_secured.bin
   ```

3. Flash the image to external flash (SDK OpenOCD flash script; J14 connected). Run from `<sdk-root>`.

   If you are running WSL, please consult the [Astra MCU SDK - WSL User Guide](../Astra_MCU_SDK_WSL_User_Guide.md) to ensure USB ports are properly handled.

   ```bash
   cd <sdk-root>
   python tools/openocd/scripts/flash_xspi_tcl.py \
     --cfg_path tools/openocd/configs/sr110_m55.cfg \
     --image examples/out/bin_files/Output/B0_Flash/B0_flash_full_image_GD25LE128_67Mhz_secured.bin \
     --erase-all
   ```

   - Use `--probe jlink` if you are flashing with J-Link.
   - Expected result: the flash tool completes without errors and the image is programmed.

4. Reset the board to boot from external flash:
   - Unplug and replug USB, or press the reset button.

## Running Examples

After flashing, reset the board and follow the README for the example you built.
