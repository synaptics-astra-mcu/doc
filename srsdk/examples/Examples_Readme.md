# Astra MCU SDK Examples - Overview

This repository contains example applications for the Synaptics SDK (Astra MCU SDK), demonstrating features and capabilities across Astra MCU platforms.

Throughout this guide, `<sdk-root>` refers to the directory where you extracted or cloned the SDK.

Scope
- This README explains examples organization and out‑of‑tree application builds.
- For setup and tool installation, see:
  - [Setup and Install SDK using CLI](../docs/Astra_MCU_SDK_Setup_and_Install_CLI.md)
  - [Setup and Install SDK using VS Code](../docs/Astra_MCU_SDK_Setup_and_Install_VsCode.md)
- For SDK structure, build system, image generation, flashing, and troubleshooting, see the [Astra MCU SDK User Guide](../docs/Astra_MCU_SDK_User_Guide.md).

---

## 📋 Table of Contents

1. [Overview](#overview)
2. [Example Organization](#example-organization)
3. [Prerequisites](#prerequisites)
4. [Directory Structure](#directory-structure)
5. [Build System Architecture](#build-system-architecture)
6. [Building Examples](#building-examples)

---

## Overview

The **Synaptics SDK (Astra MCU SDK)** provides core libraries, drivers, and build tools for Astra MCU development. This examples repository demonstrates how to build applications using the SDK in an **out-of-tree** configuration.

**Out-of-tree** means your application code lives in a separate directory from the SDK, keeping the SDK read-only and version-controlled independently.

**Key Principles:**
- **Out-of-tree examples** → Applications are maintained separately from the SDK core
- **Read-only SDK** → SDK acts as a versioned dependency, not modified directly
- **Reproducible builds** → Configuration files (defconfigs) drive consistent builds
- **Self-contained examples** → Each example can be built independently

---

## Example Organization

Examples are organized by board and category:

```
SR110_RDK/
├── configs/                    # Application defconfig files
│   ├── cm55_demo_sample_app_defconfig
│   ├── cm55_audio_example_defconfig
│   └── ...
├── audio_examples/             # Audio processing examples
│   ├── audio_capture/
│   ├── audio_playback/
│   └── ...
├── drivers_examples/           # Driver usage examples
│   ├── gpio_example/
│   ├── i2c_example/
│   ├── spi_example/
│   └── ...
├── inference_examples/         # AI/ML inference examples
│   ├── tflite_inference/
│   └── ...
├── vision_examples/            # Image processing examples
│   ├── uc_person_classification/
│   ├── object_detection/
│   └── ...
├── usb_examples/               # USB examples
└── unity_test/                 # Unit tests
SL2610_RDK/
├── configs/                    # Application defconfig files
│   ├── cm52_system_manager_rdk_defconfig
│   ├── cm52_dma_sample_app_defconfig
│   └── ...
├── drivers_examples/           # Driver usage examples
│   ├── dma_example/
│   ├── i2c_exp_example/
│   ├── spwm_example/
│   └── ...
├── system_manager/            # System Manager for SL2610_RDK

```

### Example Structure

Each example typically contains:
- **Source files** (`.c`, `.cc`, `.h`)
- **CMakeLists.txt** - Build configuration
- **kconfig** - Configuration options
- **README.md** - Example-specific documentation
- **Assets** - Models, test data, etc.

### Configuration Files (defconfigs)

Defconfigs define:
- **Board selection** (e.g., `CONFIG_BOARD="sr110_rdk"` or `CONFIG_BOARD="sl2610_rdk"`)
- **Compiler choice** (GCC or AC6)
- **Build type** (Release, Debug)
- **Enabled SDK modules** (drivers, utilities, etc.)
- **Application-specific settings**

Example defconfig: `SR110_RDK/configs/cm55_demo_sample_app_defconfig`

---

## Prerequisites

For full setup steps, see:
- [Setup and Install SDK using CLI](../docs/Astra_MCU_SDK_Setup_and_Install_CLI.md)
- [Setup and Install SDK using VS Code](../docs/Astra_MCU_SDK_Setup_and_Install_VsCode.md)

### Required Tools
- **CMake** 4.1.2
- **Ninja** 1.13.1 or later
- **Python** 3.13.x (for SDK image generation and tooling)
- **Toolchain**: GCC 13.2.1, Arm Compiler 6.19 (AC6), or LLVM Clang 21.x (SR110 only; requires GCC sysroot)

### Environment Variables

Set these environment variables before building:

```bash
# Required: SDK source directory
export SRSDK_DIR=<sdk-root>

# Required: Toolchain paths (choose one based on your compiler)
export GCC_TOOLCHAIN_13_2_1=/path/to/gcc-arm-none-eabi/bin
# OR
export AC6_TOOLCHAIN_6_19_0=/path/to/armclang/bin
# OR (SR110 + LLVM Clang)
export LLVM_TOOLCHAIN_ROOT=/path/to/llvm/bin
export GCC_TOOLCHAIN_ROOT=/path/to/gcc-arm-none-eabi
```

Use only **one** toolchain per build. LLVM builds require `GCC_TOOLCHAIN_ROOT` for the GCC sysroot and libstdc++.

**Windows (PowerShell):**
```powershell
$env:SRSDK_DIR="C:\path\to\<sdk-root>
$env:GCC_TOOLCHAIN_13_2_1="C:\path\to\gcc-arm-none-eabi\bin"
```

**VS Code:** Add to `.vscode/settings.json`:
```json
{
    "terminal.integrated.env.windows": {
        "SRSDK_DIR": "C:\\path\\to\\<sdk-root>,
        "GCC_TOOLCHAIN_13_2_1": "C:\\path\\to\\gcc-arm-none-eabi\\bin"
    }
}
```

---

## Directory Structure

```
examples/
├── build/                          # CMake build artifacts
│   └── <target>/
│       └── <compiler>/             # Build files per target/compiler
├── install/                        # Installed Astra MCU SDK Package
│   └── SR110_RDK/
│       ├── include/                # SDK headers
│       ├── lib/                    # SDK libraries (.a/.lib)
│       ├── prebuilt/               # prebuilt libraries (.a/.lib)
│       ├── config/
│       │   ├── config.h            # SDK configuration
│       │   └── *.ld/*.sct          # Linker scripts
│       └── tools/
│           └── cmake/              # Toolchain files
├── out/                            # Final binaries
│   └── <target>/
│       ├── release/                # Release builds (.elf/.axf)
│       └── debug/                  # Debug builds
├── SR110_RDK/                      # Board-specific examples
│   ├── configs/                    # Application defconfigs
│   ├── audio_examples/
│   ├── drivers_examples/
│   ├── inference_examples/
│   ├── vision_examples/
│   └── ...
├── CMakeLists.txt                  # Main build configuration
├── Makefile                        # Build orchestration
└── kconfig                         # Example-level configuration
```

---

## Build System Architecture

For build and flash workflows, see:
- [SR110 Build and Flash with CLI](../docs/SR110/SR110_Build_and_Flash_with_CLI.md)
- [SR110 Build and Flash with VS Code](../docs/SR110/SR110_Build_and_Flash_with_VSCode.md)
- [SL2610 Build and Flash with CLI](../docs/SL2610/SL2610_Build_and_Flash_with_CLI.md)
- [SL2610 Build and Flash with VS Code](../docs/SL2610/SL2610_Build_and_Flash_with_VSCode.md)

---

### Two-Stage Build Process

**Stage 1: SDK Build** (`BUILD=SRSDK`, combined SDK + app)
- Compiles SDK modules (drivers, os, soc, utilities, etc.)
- Each module produces a static library (e.g., `libdrivers.a`)
- Installs headers, libraries, and toolchain files to `install/${BOARD}/`

**Stage 2: Application Build** (`BUILD=EXAMPLE`)
- Compiles application source files
- Links against pre-built SDK libraries
- Produces final executable (`.elf` or `.axf`)

### Build Modes

- **BUILD=SRSDK** → Builds SDK libraries and the app (combined flow)
- **BUILD=EXAMPLE** → Builds application using pre-built SDK (default)
- **SDK-only** → Use `make astrasdk BOARD=<BOARD>` to build/install the SDK package without building an app

### Key Features

**Application Cache System:**
- Per-application cache of SDK headers and libraries
- Hash-based change detection (libs, config.h, toolchain)
- Automatic refresh when SDK changes
- Location: `build/<target>/<compiler>/srsdk_build/.cache/`

**Embedded SDK Build:**
- Automatically builds SDK if prebuilts are missing
- Controlled via `EMBED_SDK_BUILD` CMake option

**Configuration Management:**
- Kconfig-based configuration system
- Defconfigs for reproducible builds
- Interactive menuconfig editor

---

## Building Examples

**Important:** `SRSDK_DIR` must be set to `<sdk-root>` in any shell where you build from `<sdk-root>/examples/`, even for app‑only builds.

```bash
export SRSDK_DIR=<sdk-root>
```

For step‑by‑step workflows, use the Build and Flash guides:
- [SR110 Build and Flash with CLI](../docs/SR110/SR110_Build_and_Flash_with_CLI.md)
- [SL2610 Build and Flash with CLI](../docs/SL2610/SL2610_Build_and_Flash_with_CLI.md)

### Understanding Build Modes

- **BUILD=SRSDK**: Builds the SDK package from source, then builds your app using that fresh SDK.
  - Use when: first build, SDK source changed, or switching toolchains.
  - Slower, but ensures SDK and app are consistent.

- **BUILD=EXAMPLE** (default): Builds only your app using the previously installed SDK package.
  - Use when: SDK already built and only app code changed.
  - Faster for iterative development.

Quick commands
```bash
# One-time per shell: point examples to your SDK root (required for combined builds and installing SDK into examples)
cd <sdk-root>/examples
export SRSDK_DIR=<sdk-root>

# Build SDK + application (combined; requires SRSDK_DIR)
make <cm55_demo_sample_app_defconfig> BOARD=SR110_RDK BUILD=SRSDK

# Rebuild application only (uses installed SDK)
make build BOARD=SR110_RDK
```
