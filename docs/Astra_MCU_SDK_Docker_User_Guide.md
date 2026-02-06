# Astra MCU SDK Docker Guide (Simplified)

Use Docker to build the SDK and examples in a clean Ubuntu 22.04 environment. Flashing/debugging still runs on the host (USB access is not set up in this container by default).

Throughout this guide, `<sdk-root>` refers to your SDK checkout on the host machine.

## Prerequisites

- **Docker installed**
  - **Windows**: Use Docker Desktop with the WSL2 backend.
  - **macOS**: Use Docker Desktop or Colima.
  - **Linux**: Install Docker Engine.
- **If you need Arm Compiler 6 (AC6)**: place the AC6 tarball next to the `Dockerfile` before building.
  - Filename must match:
    - `ARMCompiler6.*linux-x86_64.tar.gz` or
    - `ARMCompiler6.*linux-aarch64.tar.gz`

> Windows note: When mounting directories from Windows into Docker, use WSL/Linux-style paths (for example `/mnt/c/Users/<name>/Astra_MCU_SDK`).

## Build the Docker image

From `<sdk-root>` (where the `Dockerfile` lives):

```bash
docker build -t <container_name> .
```

## Run the container

### If examples are inside the SDK tree (common case)

```bash
docker run --rm -it \
  -v "<sdk-root>":/workspace/srsdk \
  <container_name>
```

### If examples are in a separate folder

```bash
docker run --rm -it \
  -v "<sdk-root>":/workspace/srsdk \
  -v "<examples-root>":/workspace/examples \
  <container_name>
```

## Build examples (inside the container)

Set the SDK path for the build system:

```bash
export SRSDK_DIR=/workspace/srsdk
```

### GCC build (default)

```bash
cd /workspace/srsdk/examples
make cm55_demo_sample_app_defconfig BOARD=SR110_RDK BUILD=SRSDK
make build BOARD=SR110_RDK
```

### AC6 build (optional)

If AC6 was included during the image build, set the toolchain and build:

```bash
export AC6_TOOLCHAIN_6_19_0=/opt/arm/ac6/bin
cd /workspace/srsdk/examples
make cm55_demo_sample_app_defconfig BOARD=SR110_RDK BUILD=SRSDK
make build BOARD=SR110_RDK
```

If your examples are mounted separately, use `/workspace/examples` instead of `/workspace/srsdk/examples`.

Outputs:
- GCC/LLVM: `examples/out/sr110_cm55_fw/release/sr110_cm55_fw.elf`
- AC6: `examples/out/sr110_cm55_fw/release/sr110_cm55_fw.axf`

## SR110 image generation (inside the container)

```bash
cd /workspace/srsdk/tools/srsdk_image_generator
python srsdk_image_generator.py \
  -B0 \
  -flash_image \
  -sdk_secured \
  -spk "/workspace/srsdk/tools/srsdk_image_generator/B0_Input_examples/spk_rc4_1_0_secure_otpk.bin" \
  -apbl "/workspace/srsdk/tools/srsdk_image_generator/B0_Input_examples/sr100_b0_bootloader_ver_0x012F_ASIC.axf" \
  -m55_image "/workspace/srsdk/examples/out/sr110_cm55_fw/release/sr110_cm55_fw.elf" \
  -flash_type "GD25LE128" \
  -flash_freq "67"
```

If you built with AC6, use the `.axf` instead of `.elf` for `-m55_image`.

Output:
```
/workspace/srsdk/tools/srsdk_image_generator/Output/B0_Flash/B0_flash_full_image_GD25LE128_67Mhz_secured.bin
```

## SL2610 image generation (inside the container)

Build the System Manager app, then run the SL2610 image pipeline:

```bash
export SRSDK_DIR=/workspace/srsdk
cd /workspace/srsdk/examples
make sl2610_system_manager_rdk_defconfig BOARD=SL2610_RDK BUILD=SRSDK
make build BOARD=SL2610_RDK
make imagegen
```

Outputs:
- System Manager sub-image: `examples/out/image/intermediate/sysmgr.subimg`
- Compressed images: `examples/out/image/eMMCimg/`
- USB boot inputs: `examples/out/image/usb_boot/`

## Copy outputs to the host

Because the SDK is mounted into the container, all build artifacts already appear on the host under `<sdk-root>/examples/out/` and `<sdk-root>/tools/srsdk_image_generator/Output/`.

## Flashing and debugging

Flashing/debugging uses host USB access and is typically run on the host OS:
- SR110: `tools/openocd/scripts/flash_xspi_tcl.py`
- SL2610: `tools/usb_boot_python_tool/USB_BOOT_TOOL/usb_boot_tool.py`

See the platform guides for the full flash/debug flows:
- `docs/SR110/SR110_Build_and_Flash_with_CLI.md`
- `docs/SL2610/SL2610_Build_and_Flash_with_CLI.md`
