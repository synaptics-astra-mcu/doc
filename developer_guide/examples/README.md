# Astra MCU SDK Examples - Overview

This repository contains example applications for the Synaptics SDK (Astra MCU SDK), demonstrating various features and capabilities of the SR110 SoC.

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

The **Synaptics SDK (Astra MCU SDK)** provides core libraries, drivers, and build tools for SR110 SoC development. This examples repository demonstrates how to build applications using the SDK in an **out-of-tree** configuration.

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
- **Board selection** (e.g., `CONFIG_BOARD_SR110_RDK=y`)
- **Compiler choice** (GCC or AC6)
- **Build type** (Release, Debug, UnitTestDebug)
- **Enabled SDK modules** (drivers, utilities, etc.)
- **Application-specific settings**

Example defconfig: `SR110_RDK/configs/cm55_demo_sample_app_defconfig`

---

## Prerequisites

### Required Tools
- **CMake** 3.22 or later
- **Ninja** build system
- **Python** 3.x
- **Toolchain**: ARM GCC 13.2.1 or ARM Compiler 6 (AC6)

### Environment Variables

Set these environment variables before building:

```bash
# Required: SDK source directory
export SRSDK_DIR=/path/to/Astra MCU SDK

# Required: Toolchain paths (choose one based on your compiler)
export GCC_TOOLCHAIN_13_2_1=/path/to/gcc-arm-none-eabi
# OR
export AC6_TOOLCHAIN_6_19_0=/path/to/armclang
```

**Windows (PowerShell):**
```powershell
$env:SRSDK_DIR="C:\path\to\Astra MCU SDK"
$env:GCC_TOOLCHAIN_13_2_1="C:\path\to\gcc-arm-none-eabi"
```

**VS Code:** Add to `.vscode/settings.json`:
```json
{
    "terminal.integrated.env.windows": {
        "SRSDK_DIR": "C:\\path\\to\\Astra MCU SDK",
        "GCC_TOOLCHAIN_13_2_1": "C:\\path\\to\\gcc-arm-none-eabi"
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

---

### Two-Stage Build Process

**Stage 1: SDK Build** (`BUILD=SRSDK`)
- Compiles SDK modules (drivers, os, soc, utilities, etc.)
- Each module produces a static library (e.g., `libdrivers.a`)
- Installs headers, libraries, and toolchain files to `install/${BOARD}/`

**Stage 2: Application Build** (`BUILD=EXAMPLE`)
- Compiles application source files
- Links against pre-built SDK libraries
- Produces final executable (`.elf` or `.axf`)

### Build Modes

- **BUILD=SRSDK** → Builds SDK libraries only
- **BUILD=EXAMPLE** → Builds application using pre-built SDK (default)

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

To build examples, refer to the comprehensive build guide:

### 📖 **[build.md](build.md)** - Complete Build Instructions

The build guide covers:
- **Prerequisites** - Environment setup and toolchain configuration
- **Build Workflows** - Step-by-step instructions for all scenarios
- **Build Modes** - Understanding SDK vs application builds
- **Configuration** - Using defconfigs and menuconfig
- **Troubleshooting** - Common errors and solutions
- **Advanced Topics** - Custom configurations, multiple apps, compiler switching
- **Makefile Reference** - Complete target documentation

**Quick Commands:**
```bash
# Build SDK + application
make cm55_demo_sample_app_defconfig BOARD=SR110_RDK BUILD=SRSDK

# Build SDK only
make astrasdk BOARD=SR110_RDK

# Rebuild application
make build BOARD=SR110_RDK
```

