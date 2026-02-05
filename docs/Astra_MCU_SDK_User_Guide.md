# Astra MCU SDK User Guide

Build vision and AI products on Synaptics Astra-series MCUs—fast. Astra MCU SDK brings together optimized libraries for Arm Cortex-M–class CPUs and integrated NPUs, a production-grade build system, rich example applications, and integrated tooling to take you from first blink to deployable, secured images with minimal friction.

## Why Astra MCU SDK
- Optimized AI stack: prebuilt and source‑based TensorFlow Lite Micro with CMSIS‑NN optimizations, tuned for Arm Ethos‑U and M‑class cores.
- Rich examples: vision, inference, audio, USB, drivers.
- Out‑of‑tree examples for clean separation from SDK sources.
- Production‑ready build: Kconfig + CMake + Ninja with reproducible defconfigs, fast incremental builds, and per‑app SDK caching; GCC, Arm Compiler 6, LLVM Clang supported.
- Cross‑platform tooling: first‑class Windows, Linux, macOS support.
- VS Code extension: one‑click tool install, build, convert, flash, and debug.

---

## Contents
- [Introduction](#introduction)
- [Supported Platforms](#supported-platforms)
- [Quick Start Overview](#quick-start-overview)
- [Build Architecture](#build-architecture)
- [Build Recipes](#build-recipes)
- [Python Environment Setup](#python-environment-setup)
- [Platform-Specific Workflows](#platform-specific-workflows)
- [Troubleshooting](#troubleshooting)
- [General Notes](#general-notes)

---

## Introduction

The Astra MCU SDK provides two complementary build flows that work consistently across all supported platforms. You can run either from the VS Code extension or from the native command line.

### Build Flows

- **SDK Package (BUILD=SRSDK)**
  - Where: run in the SDK root directory.
  - What: builds the core SDK (drivers, os, soc, utilities, AI, etc.) and installs a versioned package into `examples/install/<BOARD>/` (headers, libs, toolchain CMake files, config header).
  - When: use this to produce or refresh the SDK package that example applications link against.
  - Platform notes: Some platforms may require combined SDK+App builds instead of standalone SDK builds.

- **Example + SDK (Combined) or Example‑Only**
  - Where: run in the `examples/` directory.
  - Combined build: apply the application defconfig and build SDK + app in one pass with `BUILD=SRSDK` (requires `SRSDK_DIR` to point to the SDK root). Useful when you want the SDK tailored to the app's configuration.
  - Example‑only build: fast app rebuild using the already installed SDK package (`BUILD=EXAMPLE`, default). Useful for edit‑compile‑run cycles.

### Tools: VS Code vs Native CLI

- **VS Code Extension**: "Build and Deploy" provides Build (SDK + App) vs Build App, plus Image Conversion and Image Flashing. 
- **Native CLI**: GNU Make + CMake + Ninja with Kconfig‐based configuration. Use the SDK root for SDK/bootloader/TFLite builds, and `examples/` for application builds.

### Build Outputs

- SDK package: `examples/install/<BOARD>/`
- App binaries: `examples/out/<target>/<build_type>/<target>.elf|.axf`
- Image generator outputs: `examples/out/bin_files/Output/B0_Flash/` (platform-specific)

> **Note:**
> - The examples folder does not need to live inside the SDK directory. You can keep examples in a separate repo/folder.
> - Where the SDK package installs depends on where you run `make astrasdk BOARD=<BOARD>`:
>   - From the SDK root: installs under `<sdk-root>/examples/install/<BOARD>`.
>   - From your examples folder (with `SRSDK_DIR` pointing to the SDK): installs under `<examples-root>/install/<BOARD>`.

### Environment & Toolchains

- Set `SRSDK_DIR` to the SDK root for combined builds and advanced configuration.
- Choose one compiler:
  - GCC: `GCC_TOOLCHAIN_13_2_1=/path/to/gcc-arm-none-eabi/bin`
  - Arm Compiler 6: `AC6_TOOLCHAIN_6_19_0=/path/to/armclang/bin`
  - LLVM Clang: `LLVM_TOOLCHAIN_ROOT=/path/to/llvm/bin`

---

## Supported Platforms

The Astra MCU SDK supports multiple hardware platforms across different series:

### Platform Overview

| Series | Platform | CPU Core | NPU | Development Kits |
|--------|----------|----------|-----|------------------|
| SR Series | SR110 | Cortex-M55 | Ethos-U | SR110_RDK (Astra Machina Micro) |
| SL Series | SL2610 | Cortex-M52 | None | SL2610_RDK (Astra Machina Micro) |

### Feature Comparison

| Feature | SR110 | SL2610 |
|---------|-------|--------|
| Standalone SDK Build | ✅ Supported | ✅ Supported |
| Combined SDK+App Build | ✅ Supported | ✅ Supported |
| Image Generation | Single-stage (SWD/JTAG) | Multi-stage (with Yocto SDK) |
| Flash Type | XSPI | eMMC |
| Hardware Debugging | ✅ CMSIS-DAP/J-Link | ❌ Not available |
| VS Code Full Support | ✅ Yes | ⚠️ Partial (CLI required for image gen) |
| Supported Toolchain | GCC, AC6, LLVM | GCC |

### Platform-Specific Documentation

For detailed platform-specific information, hardware setup, and workflows, please refer to:

- [SR110 Platform Guide](./SR110/SR110_platform_Guide.md#hardware-setup)
- [SL2610 Platform Guide](./SL2610/SL2610_Platform_Guide.md#hardware-setup)

---

## Quick Start Overview

### Prerequisites (All Platforms)

- Windows, Linux, or macOS
- Visual Studio Code (available on `PATH`)
- Target hardware (see platform-specific guides for details)
- Required toolchain (GCC/AC6/LLVM)

### Quick Start Path Selection

Choose your platform to begin:

<details>
<summary><b>SR110 Quick Start (VS Code)</b></summary>

See the complete [VSCode Extension User Guide for SR110](./SR110/Astra_MCU_SDK_VSCode_Extension_User_Guide_SR110.md) for:
- Hardware connection details
- Debug IC firmware update
- Build and flash workflow
- Debugging setup

**Quick Steps(VS Code):**
```bash
# Build example
Select: Device: SR110, Build Type: cm55_fw, Build Mode: Release, Board: Astra Machina Micro, Compiler: GCC, Toolchain: GCC.13.2.1, Application: demo_sample_app
Enable: Build (SDK + App)
Run

# Flash to board
Use Image Conversion → Image Flashing with CMSIS-DAP
```
</details>

<details>
<summary><b>SL2610 Quick Start (VS Code)</b></summary>

See the complete [VSCode Extension User Guide for SL2610](./SL2610/Astra_MCU_SDK_VSCode_Extension_User_Guide_SL2610.md) for:
- Power and connection setup
- UART logging configuration
- Build workflow
- eMMC flashing procedure

**Quick Steps(VS Code):**
```bash
# Build example
Select: Device: SL2610, Build Type: cm52_fw, Board: RDK
Select: Device: SL2610, Build Type: cm52_fw, Build Mode: Release, Board: RDK, Compiler: GCC, Toolchain: GCC.13.2.1, Application: sl2610_system_manager_rdk
Enable: Build (SDK + App)
Run

# Flash to board (CLI required)
Follow multi-stage image generation workflow in platform guide
```
</details>

<details>
<summary><b>Native CLI Quick Start (All Platforms)</b></summary>

**Common setup:**
```bash
# Set environment
export SRSDK_DIR=<sdk-root>
export GCC_TOOLCHAIN_13_2_1=/path/to/gcc-arm-none-eabi/bin

cd <sdk-root>/examples
```

**Platform-specific builds:**

For SR110:
```bash
make cm55_demo_sample_app_defconfig BOARD=SR110_RDK BUILD=SRSDK
make build BOARD=SR110_RDK
```

For SL2610:
```bash
make cm52_sl2610_system_manager_defconfig BOARD=SL2610_RDK BUILD=SRSDK
make build BOARD=SL2610_RDK
```

See platform guides for image generation and flashing commands.
</details>

---

## Build Architecture

### Configuration System

- **Kconfig-based**: editable via `make menuconfig` (examples) or `make advanced_menuconfig` when building SDK + app with `SRSDK_DIR` set.
- **Generators**: CMake + Ninja for fast, parallel builds.
- **Reproducible**: defconfigs ensure consistent builds across environments.

### Build Scope (BUILD)

The Build Scope defines the boundaries of the build process—whether you are compiling the entire platform or just a specific application.

**BUILD=SRSDK (SDK Package Build)**
- Builds the core SDK (drivers, os, soc, utilities, AI, etc.)
- Installs a versioned package into `examples/install/<BOARD>/`
- Package includes: headers, libraries, toolchain CMake files, config header
- Run from SDK root or examples folder (with `SRSDK_DIR` set)

**BUILD=EXAMPLE (Example-Only Build)**
- Builds only the application
- Links against the pre-installed SDK package
- Fast incremental rebuilds for edit-compile-run cycles
- Default mode when running from examples folder

### Entry Points

**SDK Root Directory:**
- Build SDK package: `make astrasdk BOARD=<BOARD>`
- Build bootloader: `make <bootloader_defconfig> BOARD=<BOARD>`
- Build TFLite: `make <tflite_defconfig> BOARD=<BOARD>`

**Examples Directory:**
- Combined build: `make <app_defconfig> BOARD=<BOARD> BUILD=SRSDK`
- App-only build: `make build BOARD=<BOARD>` (default)
- List available apps: `make list_defconfigs BOARD=<BOARD>`

### Build Artifacts

```
examples/
├── install/<BOARD>/          # SDK package (headers, libs, cmake files)
├── out/
│   ├── <target>/
│   │   ├── debug/           # Debug build outputs (.elf/.axf)
│   │   └── release/         # Release build outputs (.elf/.axf)
│   └── bin_files/
│       └── Output/          # Generated flash images (platform-specific)
```

> **Note:**
> The examples folder can live outside the SDK directory. The SDK install destination depends on where you run `make astrasdk BOARD=<BOARD>`:
> - From SDK root → `<sdk-root>/examples/install/<BOARD>`
> - From examples root (with `SRSDK_DIR=<sdk-root>`) → `<examples-root>/install/<BOARD>`

---

## Build Recipes

### SDK Package Build (SDK Root)

```bash
cd <sdk-root>
make default_config BOARD=<BOARD>
make astrasdk BOARD=<BOARD>
```

**Supported boards:**
- `SR110_RDK` (SR Series)
- `SL2610_RDK` (SL Series)

### Example Application Build (Examples)

```bash
cd <sdk-root>/examples
export SRSDK_DIR=<sdk-root>

# Combined build (SDK + App from source)
make <app_defconfig> BOARD=<BOARD> BUILD=SRSDK

# Edit configuration before building
make <app_defconfig> BOARD=<BOARD> BUILD=SRSDK EDIT=1

# Fast rebuild (app only, uses installed SDK)
make build BOARD=<BOARD>

# List available application defconfigs
make list_defconfigs BOARD=<BOARD>
```

**Common application defconfigs:**
- SR110: `cm55_demo_sample_app_defconfig`
- SL2610: `cm52_sl2610_system_manager_defconfig`

### Configuration Editing

**Advanced menuconfig (requires SRSDK_DIR):**
```bash
# Edit SDK + App configuration (full feature tree)
make advanced_menuconfig BOARD=<BOARD>

# One-shot edit + build
make <app_defconfig> BOARD=<BOARD> BUILD=SRSDK EDIT=1
```

**Examples-local menuconfig:**
```bash
# Edit application configuration only
make menuconfig
```

### Bootloader & TFLite Builds (SDK Root)

```bash
cd <sdk-root>

# Bootloader
make <bootloader_defconfig> BOARD=<BOARD>
make build

# TFLite Micro
make <tflite_defconfig> BOARD=<BOARD>
make build
```

**Platform-specific defconfigs:**
- SR110: `cm55_sr110_bootloader_defconfig`, `cm55_tflite_micro_defconfig`
- SL2610: `sl2610_bootloader_rdk_defconfig`

---

## Python Environment Setup

Many SDK tools require Python 3.13 and specific packages. Set up a virtual environment once before using image generation or flashing tools.

### Initial Setup

Refer to [Astra_MCU_SDK_Install_and_Setup](./Astra_MCU_SDK_Install_and_Setup.md) for detailed Python installation instructions.

**Create virtual environment (one-time):**

<details>
<summary><b>Windows (PowerShell)</b></summary>

```powershell
cd <sdk-root>
python -m venv .venv
.\.venv\Scripts\Activate.ps1
pip install -r tools\srsdk_image_generator\requirements.txt
```
</details>

<details>
<summary><b>Linux</b></summary>

```bash
cd <sdk-root>
python3 -m venv .venv
source .venv/bin/activate
pip install -r tools/srsdk_image_generator/requirements.txt
```
</details>

<details>
<summary><b>macOS</b></summary>

```bash
cd <sdk-root>
python3 -m venv .venv
source .venv/bin/activate
pip install -r tools/srsdk_image_generator/requirements.txt
```
</details>

### Activating the Environment

**Before running any Python-based tools**, activate the virtual environment:

**Windows:**
```powershell
C:\Users\<Username>\.sdk_venv\Scripts\Activate.ps1
```

**Linux/macOS:**
```bash
source ~/.sdk_venv/bin/activate
```

> **Note:** If using a project-local `.venv`, adjust the path accordingly.

---

## Platform-Specific Workflows

While the core build system is consistent across platforms, each platform has unique requirements for hardware setup, image generation, and flashing.

### Hardware Setup

| Platform | Power | Debug/Data | Special Notes |
|----------|-------|------------|---------------|
| SR110 | Debug IC USB (J14) | Debug IC USB (J14) | Update Debug IC firmware recommended |
| SL2610 | 5V USB-C (PWR IN) | USB 2.0 OTG | UART on Pin 8 (TX), Pin 28 (RX) |

### Image Generation

**SR110:**
- Single-stage process using `srsdk_image_generator.py`
- Outputs secured flash images for SPI flash
- Supports GD25LE128 and other SPI flash types
- See [SR110 Platform Guide](./SR110/SR110_platform_Guide.md#image-generation-2)

**SL2610:**
- Multi-stage process involving Astra_MCU_SDK and Syna-Release SDK
- Generates eMMC boot images
- Requires System Manager image
- See [SL2610 Platform Guide](./SL2610/SL2610_Platform_Guide.md#image-generation-2) and [Astra_MCU_SDK_Build_System](./Astra_MCU_SDK_Build_System.md)

### Flashing

**SR110:**
- SWD/JTAG via OpenOCD
- CMSIS-DAP or J-Link adapter
- VS Code extension or CLI
- See [SR110 Platform Guide](./SR110/SR110_platform_Guide.md#image-flashing)

**SL2610:**
- eMMC flashing via USB 2.0 OTG
- Automatic System Manager detection and installation
- CLI-based workflow
- See [SL2610 Platform Guide](./SL2610/SL2610_Platform_Guide.md#image-flashing)

### Debugging

**SR110:**
- ✅ Full hardware debugging support
- CMSIS-DAP or J-Link
- VS Code extension integration
- GDB via OpenOCD
- See [SR110 Platform Guide](./SR110/SR110_platform_Guide.md#debugging)

**SL2610:**
- ❌ Hardware debugging not available
- UART logging for diagnostics
- See [SL2610 Platform Guide](./SL2610/SL2610_Platform_Guide.md#debugging)

### Detailed Workflows

For complete step-by-step instructions, examples, and troubleshooting:

- **SR110**: [SR110 Platform Guide](./SR110/SR110_platform_Guide.md)
- **SL2610**: [SL2610 Platform Guide](./SL2610/SL2610_Platform_Guide.md)

---

## Troubleshooting

### Common Issues (All Platforms)

**"SRSDK_DIR is not set"**
```bash
# Solution: Export SDK root path
export SRSDK_DIR=/path/to/sdk-root #Linux and MacOS
$env:SRSDK_DIR="<path/to/sdk-root>" #Windows powershell
```

**"Toolchain not found"**
```bash
# Solution: Set toolchain environment variable
export GCC_TOOLCHAIN_13_2_1=/path/to/gcc-arm-none-eabi/bin
# or
export AC6_TOOLCHAIN_6_19_0=/path/to/armclang/bin
# or
export LLVM_TOOLCHAIN_ROOT=/path/to/llvm/bin
```

**Missing linker script when building examples**
```bash
# Solution: Install SDK package first
cd <sdk-root>
make astrasdk BOARD=<BOARD>
```

**No SDK libraries in install directory**
```bash
# Solution: Rebuild SDK from scratch
cd <sdk-root>/examples
make clean_package BOARD=<BOARD>
cd <sdk-root>
make astrasdk BOARD=<BOARD>
```

**PowerShell script execution error**
```
Activate.ps1 cannot be loaded because running scripts is disabled
```
```powershell
# Solution: Run PowerShell with bypass policy
powershell.exe -ExecutionPolicy Bypass
# Then run Activate.ps1
```

### Build System Issues

**Stale configuration**
```bash
# Solution: Clean and reconfigure
make clean BOARD=<BOARD>
make <app_defconfig> BOARD=<BOARD> BUILD=SRSDK
```

**SDK cache mismatch**
```bash
# Solution: Force SDK rebuild
make clean_package BOARD=<BOARD>
make build BOARD=<BOARD> BUILD=SRSDK
```

### Getting Help

If you encounter issues not covered here:
1. Check the platform-specific guide for your hardware
2. Review the [Build System Documentation](./Astra_MCU_SDK_Build_System.md)
3. Consult the example application README files
4. Contact Synaptics support with:
   - Platform and board type
   - SDK version
   - Complete error messages
   - Steps to reproduce

---

## General Notes

### Build System Optimization

- The examples build uses an app-local SDK cache for fast, reproducible builds
- SDK cache automatically refreshes when SDK inputs change
- Incremental builds detect and rebuild only changed components
- Parallel builds via Ninja maximize build performance

### Out-of-Tree Examples

- Examples folder can live outside the SDK repository
- Set `SRSDK_DIR` to point to SDK root when building from external examples
- SDK package installs relative to where you run the build command
- Enables clean separation of SDK development and application development

### Defconfig Management

- Each application has a corresponding defconfig file
- Defconfigs capture full build configuration in a single file
- Use `make list_defconfigs BOARD=<BOARD>` to see available configurations
- Create custom defconfigs by saving `.config` after menuconfig

### VS Code Extension

- Provides unified interface for all platforms
- Some features may have platform-specific limitations
- Falls back to CLI for unsupported operations
- Extension version: 1.3.0 (check for updates)

### Toolchain Notes

- GCC 13.2.1 recommended for all platforms
- Arm Compiler 6.19.0 supported on SR Series
- LLVM Clang supported on SR series 
- Mixing toolchains between SDK and application builds is not supported

---

## Additional Resources

### Documentation

- [Astra MCU SDK Build System](./Astra_MCU_SDK_Build_System.md)
- [Python Installation and Setup](./Astra_MCU_SDK_Install_and_Setup.md)
- [SR110 Platform Guide](./SR110/SR110_platform_Guide.md)
- [SL2610 Platform Guide](./SL2610/SL2610_Platform_Guide.md)

### Toolchain Setup Guides

- [Windows + GCC](./build_env/Astra_MCU_SDK_Windows_env_with_gcc.md)
- [Linux + GCC](./build_env/Astra_MCU_SDK_Linux_env_with_gcc.md)
- [macOS Arm64 + GCC](./build_env/Astra_MCU_SDK_Mac_Arm64_env_with_gcc.md)
- [macOS x86_64 + GCC](./build_env/Astra_MCU_SDK_Mac_x86_64_env_with_gcc.md)
- [Windows + LLVM](./build_env/Astra_MCU_SDK_Windows_env_with_LLVM_CLANG.md)
- [Linux + LLVM](./build_env/Astra_MCU_SDK_Linux_env_with_LLVM_CLANG.md)
- [macOS + LLVM](./build_env/Astra_MCU_SDK_Mac_env_with_LLVM_CLANG.md)
- [Windows + AC6](./build_env/Astra_MCU_SDK_Windows_env_with_ARM_Compiler.md)
- [Linux + Ac6](./build_env/Astra_MCU_SDK_Linux_env_with_ARM_Compiler.md)

### Example Applications

Each example application includes:
- README with specific usage instructions
- Defconfig file for reproducible builds
- Application-specific configuration options
- Expected outputs and verification steps

---

**Document Version:** 3.0  
**Last Updated:** January 2026  
**Supported SDK Version:** 1.3.X and later