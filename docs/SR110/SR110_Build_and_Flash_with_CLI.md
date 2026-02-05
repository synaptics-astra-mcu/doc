# SR110 Build and Flash with CLI

This document provides concise, CLI-only steps to build and flash SR110 applications.

## Prerequisites

- SR110 RDK (Astra Machina Micro) connected and powered with both USB ports.
- CLI environment, toolchains, Python tools, and OpenOCD installed. See [Setup and Install SDK using CLI](../Setup_and_Install_SDK_using_CLI.md).

## CLI Flow (Build + Flash)

1. Build the application:

   ```bash
   cd <sdk-root>/examples
   export SRSDK_DIR=<sdk-root>

   # Example application
   make cm55_demo_sample_app_defconfig BOARD=SR110_RDK BUILD=SRSDK
   make build BOARD=SR110_RDK
   ```

   The `_defconfig` you select determines which application is built. To list available defconfigs:

   ```bash
   ls *defconfig
   ```

   Example output:

   ```
   examples/out/sr110_cm55_fw/release/sr110_cm55_fw.elf
   ```

2. Generate the flash image:

   ```bash
   cd <sdk-root>/tools/srsdk_image_generator
   python srsdk_image_generator.py \
     -B0 \
     -flash_image \
     -sdk_secured \
     -spk "tools/srsdk_image_generator/B0_Input_examples/spk_rc3_0_secure_otpk_0605.bin" \
     -apbl "tools/srsdk_image_generator/B0_Input_examples/sr100_b0_bootloader_ver_0x012F_ASIC.axf" \
     -m55_image "examples/out/sr110_cm55_fw/release/sr110_cm55_fw.elf" \
     -flash_type "GD25LE128" \
     -flash_freq "67"
   ```

   Output image:

   ```
   examples/out/bin_files/Output/B0_Flash/B0_flash_full_image_GD25LE128_67Mhz_secured.bin
   ```

3. Flash the image with OpenOCD:

   The flashing script starts OpenOCD automatically.

   ```bash
   cd <sdk-root>
   python tools/openocd/scripts/flash_xspi_tcl.py \
     --cfg_path tools/openocd/configs/sr110_m55.cfg \
     --image examples/out/bin_files/Output/B0_Flash/B0_flash_full_image_GD25LE128_67Mhz_secured.bin
   ```

4. Reset the board:
   - Unplug and replug USB, or press the reset button.
   - The application should start and print logs on the serial console.

## Notes

- The script uses `openocd` from your PATH. Use `--probe jlink` if you are flashing with J-Link.
- If OpenOCD cannot connect, verify the adapter selection and cable connections.
- For more details refer: [SR110 Platform Guide](./SR110_platform_Guide.md)
