# Astra MCU SDK Build System

This document provides a technical reference for the Astra MCU SDK build system, covering Kconfig configuration, CMake/Ninja build flows, and command-line interfaces. For step-by-step platform-specific workflows (hardware setup, flashing, debugging), see the [Astra MCU SDK User Guide](./Astra_MCU_SDK_User_Guide.md) and platform guides.

---

## Contents
- [Requirements](#requirements)
- [Build System Architecture](#build-system-architecture)
- [Build Modes](#build-modes)
- [Building via Native CLI](#building-via-native-cli)
  - [Configuration Management](#configuration-management)
  - [Build SDK Package](#build-sdk-package)
  - [Build Example Applications](#build-example-applications)
  - [Build Bootloader and TFLite](#build-bootloader-and-tflite)
  - [Clean Operations](#clean-operations)
  - [Help and Reference](#help-and-reference)
- [Python Environment Setup](#python-environment-setup)
- [Image Generation and flashing](#image-generation-and-flashing)
- [VS Code Extension](#vs-code-extension)
- [Targets Reference](#targets-reference)

---

## Requirements

### Supported Platforms

The Astra MCU SDK build system supports multiple host platforms and toolchains:

**Operating Systems:**
- Windows (x64)
- Linux (Ubuntu 22.04+, x86_64 and aarch64)
- macOS (x86_64 and ARM64)

**Toolchains:**
- GCC (GCC.13.2.1)
- Arm Compiler 6 (AC6.6.18.0)
- LLVM Clang(LLVM)

### Environment Setup Guides

Before building, ensure your development environment is properly configured:

**Windows:**
- [Windows + GCC Compiler](./build_env/Astra_MCU_SDK_Windows_env_with_gcc.md)
- [Windows + ARM Compiler](./build_env/Astra_MCU_SDK_Windows_env_with_ARM_Compiler.md)
- [Windows + LLVM Clang](./build_env/Astra_MCU_SDK_Windows_env_with_LLVM_CLANG.md)

**Linux:**
- [Linux + GCC Compiler](./build_env/Astra_MCU_SDK_Linux_env_with_gcc.md)
- [Linux + ARM Compiler](./build_env/Astra_MCU_SDK_Linux_env_with_ARM_Compiler.md)
- [Linux + LLVM Clang](./build_env/Astra_MCU_SDK_Linux_env_with_LLVM_CLANG.md)

**macOS:**
- [macOS ARM64 + GCC](./build_env/Astra_MCU_SDK_Mac_Arm64_env_with_gcc.md)
- [macOS x86_64 + GCC](./build_env/Astra_MCU_SDK_Mac_x86_64_env_with_gcc.md)
- [macOS + LLVM Clang](./build_env/Astra_MCU_SDK_Mac_env_with_LLVM_CLANG.md)

**Additional Setup:**
- [Setup and Install SDK using CLI](./Setup_and_Install_SDK_using_CLI.md)
- [Setup and Install SDK using VS Code Extension](./Setup_and_Install_SDK_using_VSCode.md)
- [VS Code Extension User Guide](./Astra_MCU_SDK_VSCode_Extension_User_Guide.md)

---

## Build System Architecture

### Core Technologies

The Astra MCU SDK uses a modern, layered build system:

**Configuration Layer:**
- **Kconfig**: Menuconfig-based configuration system
- Drives feature selection and build parameters
- Generates configuration headers and build flags

**Build Layer:**
- **CMake**: Cross-platform build system generator
- **Ninja**: Fast, parallel build execution
- **GNU Make**: Top-level orchestration

### Build Structure

```
SDK Root (this repository)
├── configs/                    # Defconfig files
│   ├── <BOARD>/
│   │   ├── default_package_defconfig
│   │   ├── <bootloader>_defconfig
│   │   └── <tflite>_defconfig
│   └── ...
├── kconfig                     # Master Kconfig entry
├── Makefile                    # Top-level SDK Makefile
├── build/                      # Build artifacts (SDK root)
│   ├── config.h               # Generated configuration header
│   └── <project>/<compiler>/  # Compiled objects
└── out/                        # SDK root build outputs
    ├── <target>/<type>/       # Binaries (.elf/.axf)
    └── ...

Examples (out-of-tree)
├── configs/                    # Application defconfigs
│   ├── <BOARD>/
│   │   ├── <app1>_defconfig
│   │   ├── <app2>_defconfig
│   │   └── ...
│   └── ...
├── kconfig                     # Examples Kconfig overlay
├── Makefile                    # Examples Makefile
├── install/                    # Installed SDK packages
│   └── <BOARD>/
│       ├── include/           # SDK headers
│       ├── lib/               # SDK libraries
│       └── cmake/             # Toolchain files
├── build/                      # Application build cache
└── out/                        # Application build outputs
    ├── <target>/<type>/       # Application binaries
    └── bin_files/             # Generated images (platform-specific)
```

### Directory Roles

**SDK Root:**
- Contains core SDK sources (drivers, OS, SoC, utilities)
- Hosts bootloader and TFLite builds
- Installs SDK packages to `<sdk-root>/examples/install/<BOARD>/`

**Examples Directory:**
- Out-of-tree application repository
- Can live outside SDK repository
- Uses installed SDK packages for building
- When `SRSDK_DIR` is set, can also build and install SDK packages to `<examples-root>/install/<BOARD>/`

---

## Build Modes

The SDK supports two distinct build modes controlled by the `BUILD` variable:

### BUILD=SRSDK (SDK Package Build)

**Purpose:** Build the core SDK and install it as a reusable package.

**What it includes:**
- Core libraries (drivers, OS, SoC, utilities)
- AI libraries (TensorFlow Lite Micro, CMSIS-NN)
- SDK headers
- CMake toolchain files
- Generated configuration header

**When to use:**
- Creating or updating the SDK package
- Changing SDK-level configuration
- Building SDK with application-specific settings (combined build)

**Installation location:**
- From SDK root: `<sdk-root>/examples/install/<BOARD>/`
- From examples (with `SRSDK_DIR`): `<examples-root>/install/<BOARD>/`

**Example:**
```bash
# SDK root
make default_config BOARD=<BOARD>
make astrasdk BOARD=<BOARD>

# Examples (combined build)
export SRSDK_DIR=<sdk-root>
make <app_defconfig> BOARD=<BOARD> BUILD=SRSDK
```

### BUILD=EXAMPLE (Application-Only Build)

**Purpose:** Build applications using pre-installed SDK package.

**What it includes:**
- Application source code only
- Links against installed SDK libraries
- Fast incremental rebuilds

**When to use:**
- Edit-compile-run cycles
- SDK package already installed
- No SDK-level changes needed

**Requirements:**
- SDK package must be installed first
- Package location: `<examples-root>/install/<BOARD>/`

**Example:**
```bash
cd <examples-root>
make build BOARD=<BOARD>
```

> **Note:** Platform-specific build instructions are documented in the respective platform guides. See [SR110 Platform Guide](./SR110/SR110_platform_Guide.md#quick-start-native-cli) and [SL2610 Platform Guide](./SL2610/SL2610_Platform_Guide.md#quick-start-native-cli).

---

## Building via Native CLI

### Configuration Management

#### Configuration Files

**Defconfigs:**
- Minimal configuration files that capture essential settings
- Located in `configs/<BOARD>/`
- Applied using `make <name>_defconfig BOARD=<BOARD>`

**.config:**
- Full configuration file generated from defconfig
- Contains all Kconfig options (defaults + user selections)
- Used by build system to generate headers and build flags

**config.h:**
- Generated C header from `.config`
- Consumed by source code
- Located in `build/config.h` (SDK root) or application build directory

#### SDK Root Configuration

**Apply default SDK package configuration:**
```bash
cd <sdk-root>
make default_config BOARD=<BOARD>
```
This loads `configs/<BOARD>/default_package_defconfig` and writes `.config`.

**Apply bootloader or TFLite configuration:**
```bash
# Bootloader
make <bootloader_defconfig> BOARD=<BOARD>

# TFLite
make <tflite_defconfig> BOARD=<BOARD>
```

**Interactive configuration:**
```bash
make menuconfig BOARD=<BOARD>
```
Opens the Kconfig UI, allowing you to browse and modify options.

**List available defconfigs:**
```bash
make list_defconfigs BOARD=<BOARD>
```

**Save custom defconfig:**
```bash
make savedefconfig OUT=my_config BOARD=<BOARD>
```
Saves minimal configuration to `configs/<BOARD>/my_config_defconfig`.

#### Examples Configuration

**Two Kconfig Entry Points:**

1. **Examples-Local Kconfig** (`examples/kconfig`):
   - Application-level options only
   - Does not require `SRSDK_DIR`
   - Invoked by `make menuconfig` in examples directory
   - Use for: Application settings, example-specific options

2. **Advanced Kconfig** (SDK master at `$SRSDK_DIR/kconfig`):
   - Full SDK option set + application overlay
   - Requires `SRSDK_DIR` to be set
   - Invoked by `make advanced_menuconfig` or `EDIT=1`
   - Use for: SDK-level changes (drivers, compiler, build modes, etc.)

**Apply application defconfig:**
```bash
cd <examples-root>
make <app_defconfig> BOARD=<BOARD>
```

**One-shot edit and build:**
```bash
export SRSDK_DIR=<sdk-root>
make <app_defconfig> BOARD=<BOARD> BUILD=SRSDK EDIT=1
```
Opens advanced Kconfig UI first, then builds SDK + application.

**Edit configuration only (advanced):**
```bash
export SRSDK_DIR=<sdk-root>
make advanced_menuconfig BOARD=<BOARD>
```
Opens combined SDK+example Kconfig UI. Build later with `make build`.

**Edit configuration only (examples-local):**
```bash
make menuconfig BOARD=<BOARD>
```
Opens examples tree Kconfig only. No `SRSDK_DIR` required.

**List application defconfigs:**
```bash
make list_defconfigs BOARD=<BOARD>
```

**Save custom application defconfig:**
```bash
# Edit .config via menuconfig
make menuconfig BOARD=<BOARD>

# Save to configs/<BOARD>/my_app_defconfig
cp .config configs/<BOARD>/my_app_defconfig
```

#### Configuration Workflow Summary

```
┌─────────────────────────────────────┐
│  Select Defconfig                   │
│  make <name>_defconfig BOARD=...    │
└──────────────┬──────────────────────┘
               │
               ▼
┌─────────────────────────────────────┐
│  .config Generated                  │
│  (Full configuration)               │
└──────────────┬──────────────────────┘
               │
               ▼
┌─────────────────────────────────────┐
│  Optional: Edit Configuration       │
│  make menuconfig / advanced_menu... │
└──────────────┬──────────────────────┘
               │
               ▼
┌─────────────────────────────────────┐
│  Build System Generates config.h    │
│  (Consumed by source code)          │
└──────────────┬──────────────────────┘
               │
               ▼
┌─────────────────────────────────────┐
│  Build Proceeds                     │
│  make build / make astrasdk         │
└─────────────────────────────────────┘
```

---

### Build SDK Package

**Purpose:** Build and install the SDK package for use by applications.

**Prerequisites:**
- Toolchain installed and in PATH
- Environment variables set (see [Requirements](#requirements))

**Steps:**

1. **Apply configuration:**
   ```bash
   cd <sdk-root>
   make default_config BOARD=<BOARD>
   ```

2. **Build and install SDK package:**
   ```bash
   make astrasdk BOARD=<BOARD>
   ```

**Output:**
- SDK package installed to `<sdk-root>/examples/install/<BOARD>/`
- Contains:
  - `include/` - SDK headers
  - `lib/` - Compiled libraries
  - `cmake/` - Toolchain CMake files
  - `config.h` - Configuration header

**Supported Boards:**
```bash
# Current supported boards (check with list_defconfigs)
SR110_RDK       # SR Series
SL2610_RDK      # SL Series
```

**Example Output Structure:**
```
examples/install/<BOARD>/
├── include/
│   ├── drivers/
│   ├── os/
│   ├── soc/
│   └── ...
├── lib/
│   ├── libsrsdk_core.a
│   ├── libsrsdk_drivers.a
│   └── ...
├── cmake/
│   └── toolchain_<compiler>.cmake
└── config.h
```

---

### Build Example Applications

**Purpose:** Build application binaries using the SDK package.

**Prerequisites:**
- SDK package installed (see [Build SDK Package](#build-sdk-package))
- For combined builds: `SRSDK_DIR` environment variable set

#### Combined Build (SDK + Application)

Builds SDK package and application in one step.

```bash
cd <examples-root>
export SRSDK_DIR=<sdk-root>
make <app_defconfig> BOARD=<BOARD> BUILD=SRSDK
```

**When to use:**
- First build of an application
- SDK needs rebuilding with app-specific configuration
- SDK sources have changed

**Output:**
- SDK package: `<examples-root>/install/<BOARD>/`
- Application binary: `<examples-root>/out/<target>/<type>/<target>.elf`

#### Application-Only Build

Fast rebuild using pre-installed SDK package.

```bash
cd <examples-root>
make build BOARD=<BOARD>
```

**When to use:**
- Iterative development (edit-compile-run)
- SDK package already installed
- Only application sources changed

**Output:**
- Application binary: `<examples-root>/out/<target>/<type>/<target>.elf`

#### List Available Applications

```bash
cd <examples-root>
make list_defconfigs BOARD=<BOARD>
```

**Example output:**
```
Available defconfigs for SR110_RDK:
  cm55_demo_sample_app_defconfig
  cm55_person_detection_defconfig
  cm55_pinmux_sample_app_defconfig
  ...

Available defconfigs for SL2610_RDK:
  cm52_sl2610_system_manager_rdk_defconfig
  cm52_dma_sample_app_rdk_defconfig
  ...
```
---

### Build Bootloader and TFLite

**Purpose:** Build bootloader or TensorFlow Lite Micro libraries from SDK root.

**Location:** SDK root directory only (not in examples)

**Prerequisites:**
- Toolchain installed and in PATH

#### Build Bootloader

```bash
cd <sdk-root>
make <bootloader_defconfig> BOARD=<BOARD>
make build
```

#### Build TensorFlow Lite Micro

```bash
cd <sdk-root>
make <tflite_defconfig> BOARD=<BOARD>
make build
```

**Output:**
- Libraries: `prebuilt/release/`

> **Important Notes:**
> - TFLite libraries required for vision applications are included in SDK package
> - Applications automatically link against installed TFLite

---

### Clean Operations

#### Clean Build Artifacts

**Remove build outputs (SDK root):**
```bash
cd <sdk-root>
make clean
```
Removes: `build/<project>/<compiler>/` and generated headers (`build/*.h`)

**Remove build outputs (examples):**
```bash
cd <examples-root>
make clean BOARD=<BOARD>
```
Removes: Application build directory

#### Clean SDK Package

**Remove installed SDK package:**
```bash
cd <examples-root>
make clean_package BOARD=<BOARD>
```
Removes: `install/<BOARD>/`

#### Complete Clean and Rebuild

**Full clean and rebuild workflow:**
```bash
cd <examples-root>

# Clean everything
make clean_package BOARD=<BOARD>
make clean BOARD=<BOARD>

# Rebuild from scratch
export SRSDK_DIR=<sdk-root>
make <app_defconfig> BOARD=<BOARD> BUILD=SRSDK
make build BOARD=<BOARD>
```

---

### Help and Reference

#### Display Help Information

**SDK root help:**
```bash
cd <sdk-root>
make help
```

**Example output:**
```
Usage: make [target] [BOARD=value]

Common targets:
  help                   Show this help message
  list_defconfigs        List available defconfigs
  menuconfig             Run menuconfig UI
  genconfig              Generate autoconf headers
  build                  Build the project
  default_config         Apply default package defconfig
  astrasdk               Build and install SDK package
  savedefconfig          Save minimal defconfig
  clean                  Remove build artifacts

Variables:
  BOARD=<board>          Board name (required)

Examples:
  make default_config BOARD=SR110_RDK
  make astrasdk BOARD=SR110_RDK
  make cm55_sr110_bootloader_defconfig BOARD=SR110_RDK
  make build
```

**Examples help:**
```bash
cd <examples-root>
make help
```

**Example output:**
```
Usage: make [target] [BOARD=<board>] [BUILD={EXAMPLE|SRSDK}] [EDIT=1]

Build Modes:
  BUILD=EXAMPLE        Default. Build app using installed SDK.
  BUILD=SRSDK          Build SDK package (requires SRSDK_DIR).

Common Targets:
  <app_defconfig>      Apply app defconfig and build
  build                Build the application
  astrasdk             Build default SDK package
  custom_astrasdk      Build custom SDK package
  list_defconfigs      List available app defconfigs
  menuconfig           Run app-local menuconfig
  advanced_menuconfig  Run advanced Kconfig (requires SRSDK_DIR)
  clean                Remove build outputs
  clean_package        Remove installed SDK

Notes:
  - Set SRSDK_DIR for BUILD=SRSDK or advanced targets
  - Valid boards: SR110_RDK, SL2610_RDK, SL2610_PEK
  - Default BUILD mode is EXAMPLE

Examples:
  make cm55_demo_sample_app_defconfig BOARD=SR110_RDK BUILD=SRSDK
  make build BOARD=SR110_RDK
  make list_defconfigs BOARD=SL2610_RDK
```

---

## Python Environment Setup

Many SDK tools (image generation, flashing) require Python 3.13 and specific packages.

### Prerequisites

- Python 3.13 installed
- `pip` package manager

### Create Virtual Environment

**Linux/macOS:**
```bash
cd <sdk-root>
python3 -m venv .venv
source .venv/bin/activate
pip install -r tools/srsdk_image_generator/requirements.txt
```

**Windows (PowerShell):**
```powershell
cd <sdk-root>
python -m venv .venv
.\.venv\Scripts\Activate.ps1
pip install -r tools\srsdk_image_generator\requirements.txt
```

**PowerShell Execution Policy:**

If you encounter: `Activate.ps1 cannot be loaded because running scripts is disabled`

```powershell
# Run PowerShell with bypass policy
powershell.exe -ExecutionPolicy Bypass

# Then activate
.\.venv\Scripts\Activate.ps1
```

### Activate Environment

**Before running any Python-based tools**, activate the virtual environment:

**Linux/macOS:**
```bash
source ~/.sdk_venv/bin/activate
# or project-local
source .venv/bin/activate
```

**Windows:**
```powershell
C:\Users\<Username>\.sdk_venv\Scripts\Activate.ps1
# or project-local
.\.venv\Scripts\Activate.ps1
```

**Verification:**
```bash
# Check Python version
python --version  # Should show 3.13.x

# Check packages installed
pip list | grep pycryptodome
```

### Requirements

The `requirements.txt` typically includes:
- `pycryptodome` - Cryptographic operations for secure images
- `pyserial` - Serial port communication
- Platform-specific dependencies

For complete installation instructions, see:
  - [Setup and Install SDK using CLI](./Setup_and_Install_SDK_using_CLI.md)
  - [Setup and Install SDK using VS Code Extension](./Setup_and_Install_SDK_using_VSCode.md)

---

## Image Generation and flashing

> **Platform-Specific Note:** Image generation and flashing workflows differs between platforms. This section covers general concepts. For platform-specific commands and procedures, see:

> - [SR110 Platform Guide - Image Generation](./SR110/SR110_platform_Guide.md#image-generation-2)
> - [SL2610 Platform Guide - Image Generation](./SL2610/SL2610_Platform_Guide.md#image-generation-2)

---

## VS Code Extension

The Astra MCU SDK provides a VS Code extension for integrated development workflows.

### Extension Features

**Build:**
- Visual configuration selection
- Build (SDK + App) and Build App modes
- Interactive menuconfig
- Build output integration

**Image Conversion:**
- Platform-specific image generation
- Parameter selection via UI
- Auto-population of build outputs

**Flashing:**
- Platform-appropriate flash methods
- Port detection and selection
- Progress monitoring

**Debugging (SR110 only):**
- GDB integration
- Breakpoint management
- Variable inspection
- Memory viewing

### Extension Installation

```bash
code --install-extension <sdk-root>/tools/Astra_MCU_SDK_vscode_extension-1.3.0.vsix
```

### Documentation

For complete extension documentation:
- **SR110**: [VS Code Extension User Guide (SR110)](./SR110/Astra_MCU_SDK_VSCode_Extension_User_Guide_SR110.md)
- **SL2610**: [VS Code Extension User Guide (SL2610)](./SL2610/Astra_MCU_SDK_VSCode_Extension_User_Guide_SL2610.md)

---

## Targets reference (quick glance)

SDK root (this repository)
- `default_config`:
  - Description: Applies the default SDK package configuration (loads `configs/<BOARD>/default_package_defconfig` and writes `.config`).
  - Inputs: `BOARD` (required).
  - Outputs: Updated `.config` in the SDK root.
  - Example: `make default_config BOARD=<BOARD>`
  - When to use: Prepare to build a standard SDK package with `make astrasdk BOARD=<BOARD>`.

- `<name>_defconfig` (root):
  - Description: Applies a specific SDK/root defconfig from `configs/<BOARD>/` (e.g., bootloader or TFLite presets) and writes `.config`.
  - Inputs: `BOARD` (required), target name (e.g., `cm55_sr110_bootloader_defconfig`).
  - Outputs: Updated `.config` in the SDK root.
  - Example: `make cm55_sr110_bootloader_defconfig BOARD=<BOARD>`
  - When to use: Select the bootloader or TFLite configuration prior to `make build`.

- `astrasdk`:
  - Description: Builds and installs the SDK package using the current `.config` (package install includes headers, libraries, toolchain CMake files, and `config.h`).
  - Inputs: `BOARD` (required); relies on `.config` (use `default_config` or another defconfig first).
  - Outputs: SDK package under `<sdk-root>/examples/install/<BOARD>/`.
  - Example: `make astrasdk BOARD=<BOARD>`
  - When to use: Produce or refresh the SDK package that applications link against.

- `build`:
  - Description: Builds the currently selected SDK root project based on `.config` (e.g., bootloader, TFLite, or firmware), generating the target binary but not installing a package.
  - Inputs: `.config` (produced by a defconfig step), implicit target derived from Kconfig.
  - Outputs: Binaries under `out/<target>/<build_type>/` (e.g., `out/sr110_bootloader/release/sr110_bootloader.elf`).
  - Example: `make build`
  - When to use: Compile bootloader or TFLite from the SDK root when you don’t need the installed package.

- `menuconfig` (root):
  - Description: Opens the SDK root Kconfig UI and writes selections to `.config`.
  - Inputs: None (besides current SDK tree).
  - Outputs: Updated `.config` in the SDK root.
  - Example: `make menuconfig`
  - When to use: Tweak SDK‑level options (drivers, SoC features, compiler, build type) before building.

- `list_defconfigs`:
  - Description: Lists available SDK/root defconfigs under `configs/<BOARD>/`.
  - Inputs: `BOARD` (required).
  - Outputs: Console listing.
  - Example: `make list_defconfigs BOARD=<BOARD>`
  - When to use: Discover available bootloader/TFLite or other SDK presets.

- `savedefconfig`:
  - Description: Saves the current `.config` as a minimal defconfig under `configs/<BOARD>/`.
  - Inputs: `BOARD` (required), `OUT=<name>` (optional; defaults to `defconfig`).
  - Outputs: `configs/<BOARD>/<name>_defconfig`.
  - Example: `make savedefconfig OUT=my_sdk BOARD=<BOARD>`
  - When to use: Capture and version your current SDK configuration as a preset.

- `genconfig` (root):
  - Description: Generates the SDK configuration header `build/config.h` from `.config` without invoking a full build.
  - Inputs: Existing `.config`.
  - Outputs: `build/config.h`.
  - Example: `make genconfig`
  - When to use: Rare; if you need the header generated from config without compiling (normal builds regenerate as needed).

- `clean` / `help`:
  - Description: Maintenance and introspection.
    - `clean`: removes `build/<project>/<compiler>` and generated headers (`build/*.h`).
    - `help`: prints usage and common targets.
  - Inputs: N/A.
  - Outputs: Console text (for `help`); filesystem cleanup (for `clean`).
  - Example: `make clean`; `make help`
  - When to use: Reset root build state or discover available commands.

Common variables
- `SRSDK_DIR`: Path to SDK root (required in examples for combined builds and SDK install).
