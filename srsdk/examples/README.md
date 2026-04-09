# Astra MCU SDK Examples - Overview

This repository contains example applications for the Synaptics SDK (Astra MCU SDK), demonstrating features and capabilities across Astra MCU platforms.

Throughout this guide, `<sdk-root>` refers to the directory where you extracted or cloned the SDK.

Scope
- This README explains examples organization and the latest application build flow.
- For setup and tool installation, see:
  - [Setup and Install SDK using CLI](../docs/Astra_MCU_SDK_Setup_and_Install_CLI.md)
  - [Setup and Install SDK using VS Code](../docs/Astra_MCU_SDK_Setup_and_Install_VsCode.md)
- For SDK structure, build system, image generation, flashing, and troubleshooting, see the [Astra MCU SDK User Guide](../docs/Astra_MCU_SDK_User_Guide.md).

---

## Table of Contents

1. [Overview](#overview)
2. [Example Organization](#example-organization)
3. [Prerequisites](#prerequisites)
4. [Directory Structure](#directory-structure)
5. [Build System Architecture](#build-system-architecture)
6. [Building Examples](#building-examples)

---

## Overview

The **Synaptics SDK (Astra MCU SDK)** provides core libraries, drivers, and build tools for Astra MCU development. This examples repository shows how applications are organized and built with the current application build system.

Applications now use a unified structure:
- Each application is a single app directory.
- Board-specific files are handled under `hw/<BOARD>/`.
- Application defconfigs are stored under the app's own `configs/` directory.
- The same application can support multiple boards without duplicating the full app tree.

Applications can be built:
- **In-tree**, directly from their folder under `<sdk-root>/examples/...`
- **Out-of-tree**, by exporting or importing the application as a separate repository and setting `SRSDK_DIR=<sdk-root>`

In both cases, builds are run from the **application folder itself**.

**Key Principles:**
- **Unified applications** → One app directory can support multiple boards
- **Board-specific hardware layer** → Board and hardware setup lives in `hw/<BOARD>/`
- **Reproducible builds** → Defconfigs drive consistent application builds
- **Standalone app support** → Applications can be built independently outside the SDK tree by using `SRSDK_DIR`

---

## Example Organization

Examples are organized by functional category:

```
examples/
├── audio_examples/
├── driver_examples/
├── inference_examples/
├── usb_examples/
├── vision_examples/
├── sample_demo_app/
├── system_manager/
└── unity_test/
```

### Example Structure

Each example typically contains:
- **Source files** (`.c`, `.cc`, `.h`)
- **CMakeLists.txt** - Application CMake entry
- **Makefile** - Application build wrapper
- **kconfig** - Application configuration options
- **src.cmake** - Application source list
- **configs/** - Application defconfigs
- **hw/<BOARD>/** - Board-specific init and helper files
- **README.md** - Example-specific documentation
- **Assets** - Models, test data, or other app content where applicable

### Configuration Files (defconfigs)

Application defconfigs define:
- **Board selection** (for example `CONFIG_BOARD="sr110_rdk"`)
- **Compiler choice** (GCC, AC6, or LLVM where supported)
- **Build type** (`cm55_fw`, `cm52_fw`, and other app target types as applicable)
- **Enabled SDK modules**
- **Application-specific settings**

Each application stores its supported defconfigs locally under its own `configs/` directory.

The selected defconfig also determines which board-specific implementation is used from `hw/<BOARD>/`.

---

## Prerequisites

For full setup steps, see:
- [Setup and Install SDK using CLI](../docs/Astra_MCU_SDK_Setup_and_Install_CLI.md)
- [Setup and Install SDK using VS Code](../docs/Astra_MCU_SDK_Setup_and_Install_VsCode.md)

### Required Tools
- **CMake** 4.1.2
- **Ninja** 1.13.1 or later
- **Python** 3.13.x (for configuration tools and image generation)
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
$env:SRSDK_DIR="C:\path\to\<sdk-root>"
$env:GCC_TOOLCHAIN_13_2_1="C:\path\to\gcc-arm-none-eabi\bin"
```

**VS Code:** Add to `.vscode/settings.json`:
```json
{
    "terminal.integrated.env.windows": {
        "SRSDK_DIR": "C:\\path\\to\\<sdk-root>",
        "GCC_TOOLCHAIN_13_2_1": "C:\\path\\to\\gcc-arm-none-eabi\\bin"
    }
}
```

For exported applications, `SRSDK_DIR` must point to a valid SDK root that contains `tools/make/example_app.mk`.

---

## Directory Structure

```
examples/
├── README.md                       # Examples overview and build guidance
├── audio_examples/
│   └── <app>/                      # Application
├── driver_examples/
│   └── <app>/                      # Application
│       ├── configs/                # App defconfigs
│       ├── hw/
│       │   └── <BOARD>/            # Board-specific init and helper files
│       ├── CMakeLists.txt          # Application CMake entry
│       ├── Makefile                # Application build wrapper
│       ├── kconfig                 # Application config options
│       ├── src.cmake               # Application source list
│       ├── README.md               # App-specific guide
│       └── <sources>               # App sources and headers
├── inference_examples/
│   └── <app>/
├── usb_examples/
│   └── <app>/
├── vision_examples/
│   └── <app>/
├── sample_demo_app/
├── system_manager/
└── unity_test/
```

After an application is built, its local build outputs are created inside that application directory:

```
<app-dir>/
├── build/                          # Build tree and generated config.h
├── install/                        # App-local installed SDK package
│   └── <BOARD>/<BUILD_TYPE>/
├── out/                            # Final application binaries
│   └── <target>/<mode>/
├── configs/
├── hw/<BOARD>/
├── CMakeLists.txt
├── Makefile
├── kconfig
└── src.cmake
```

If an application is built against a preinstalled SDK package, it also uses:

```
<sdk-root>/install/<BOARD>/<BUILD_TYPE>/
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
- Compiles SDK modules needed by the application
- Installs headers, libraries, toolchain files, and `config.h` to the app-local SDK package under `install/<BOARD>/<BUILD_TYPE>/`

**Stage 2: Application Build** (`BUILD=EXAMPLE`)
- Compiles application source files
- Links against the selected SDK package
- Produces the final executable (`.elf` or `.axf`) under `out/<target>/<mode>/`

### Build Modes

- **BUILD=SRSDK** → Builds the SDK package and the app together, using the application directory as the entry point
- **BUILD=EXAMPLE** → Builds the application using an installed SDK package (default)
- **BUILD=NONE** → Applies the defconfig only and skips the build

### SDK Package Options

- **App-local SDK package**
  - Installed under `<app-dir>/install/<BOARD>/<BUILD_TYPE>/`
  - Created by `BUILD=SRSDK`, `build_sdk`, or `build_all`

- **Preinstalled SDK package**
  - Installed under `<sdk-root>/install/<BOARD>/<BUILD_TYPE>/`
  - Used with `USE_PREINSTALLED_SDK=1`
  - Typically created earlier from the SDK root

### Key Features

**Application Cache System:**
- Per-application cache of SDK headers and libraries
- Hash-based change detection for SDK content
- Automatic refresh when SDK content changes
- Location: `build/<target>/<compiler>/srsdk_build/.cache/`

**Standalone Application Flow:**
- Applications can be exported or imported as a separate repository
- The same app structure works outside the SDK tree
- `SRSDK_DIR` points the app to the SDK root

**Configuration Management:**
- Kconfig-based configuration system
- Defconfigs for reproducible builds
- Interactive `menuconfig` editor

---

## Building Examples

**Important:** `SRSDK_DIR` must be set in any shell where you build an application, whether the app is inside `examples/` or in a separate repository.

```bash
export SRSDK_DIR=<sdk-root>
```

For step-by-step workflows, use the Build and Flash guides:
- [SR110 Build and Flash with CLI](../docs/SR110/SR110_Build_and_Flash_with_CLI.md)
- [SL2610 Build and Flash with CLI](../docs/SL2610/SL2610_Build_and_Flash_with_CLI.md)

### Understanding Build Modes

- **BUILD=SRSDK**: Builds the app-local SDK package and then builds the app.
  - Use when: first build, SDK source changed, toolchain changed, or app-local SDK package does not exist.

- **BUILD=EXAMPLE** (default): Builds only the app using the selected SDK package.
  - Use when: the required SDK package is already available and only app-side changes are being made.

- **USE_PREINSTALLED_SDK=1**: Uses the SDK package already installed under `SRSDK_DIR/install/<BOARD>/<BUILD_TYPE>/`.

### Quick Commands

```bash
# One-time per shell: point the application to your SDK root
export SRSDK_DIR=<sdk-root>

# Change to an individual application directory
cd <sdk-root>/examples/<example_type>/<app>

# List supported defconfigs for the app
make list_defconfigs

# Apply a defconfig only
make <app_defconfig> BUILD=NONE

# Build SDK + app (combined; app-local SDK package)
make <app_defconfig> BUILD=SRSDK

# Rebuild app only using the app-local SDK package
make build

# Build app using a preinstalled SDK package from SRSDK_DIR/install/...
make <app_defconfig> BUILD=EXAMPLE USE_PREINSTALLED_SDK=1
```

### Menuconfig and Custom Defconfigs

```bash
# Load a defconfig without building
make <app_defconfig> BUILD=NONE

# Open the interactive config menu
make menuconfig

# Save the current config as a new defconfig
make savedefconfig OUT=my_custom_app
```

This writes:

```text
configs/my_custom_app_defconfig
```

### Using Different Compilers

To change the compiler for an application build, update the application configuration so that both the compiler selection and toolchain selection match, then rebuild the SDK package and the application.

**Switch from GCC to AC6:**
1. Apply the app defconfig or open `menuconfig`.
2. In `SRSDK BUILD CONFIGURATION -> COMPILER SELECTION`, clear `gcc` and select `ac6`.
3. In `SRSDK BUILD CONFIGURATION -> Toolchain SELECTION`, clear `GCC.13.2.1` and select `AC6.6.18.0`.
4. Rebuild the SDK package and the application:
   ```bash
   make <app_defconfig> BUILD=SRSDK
   ```

**Switch from GCC to LLVM:**
1. Apply the app defconfig or open `menuconfig`.
2. In `SRSDK BUILD CONFIGURATION -> COMPILER SELECTION`, clear `gcc` and select `LLVM`.
3. In `SRSDK BUILD CONFIGURATION -> Toolchain SELECTION`, clear `GCC.13.2.1` and select `LLVM`.
4. Rebuild the SDK package and the application:
   ```bash
   make <app_defconfig> BUILD=SRSDK
   ```

Notes:
- Keep `CONFIG_COMPILER` and `CONFIG_TOOLCHAIN` aligned.
- Make sure the matching toolchain environment variables are set before rebuilding.
- LLVM application builds are supported only for the SR110 workflows.

### Exported or Separate Application Repository

Applications can also be built outside `<sdk-root>/examples/` as a separate repository.

In that case:
- Keep the application structure the same (`configs/`, `hw/<BOARD>/`, `Makefile`, `CMakeLists.txt`, `kconfig`, `src.cmake`)
- Set `SRSDK_DIR=<sdk-root>`
- Build from the exported application folder itself

Example:

```bash
cd <exported-app-dir>
export SRSDK_DIR=<sdk-root>
make list_defconfigs
make <app_defconfig> BUILD=SRSDK
```
