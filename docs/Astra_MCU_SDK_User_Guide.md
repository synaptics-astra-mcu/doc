# Astra MCU SDK Reference

This document is a comprehensive reference for the Astra MCU SDK. It describes the SDK structure, build and configuration model, project layout, image generation, flashing tools, and common troubleshooting. It is not a quick start guide and focuses on SDK internals and CLI workflows.

Throughout this guide, `<sdk-root>` refers to the directory where you extracted or cloned the SDK.

## Table of Contents

- [Scope and Audience](#scope-and-audience)
- [Supported Platforms and Boards](#supported-platforms-and-boards)
- [Host Requirements](#host-requirements)
- [Environment Variables and Toolchain Selection](#environment-variables-and-toolchain-selection)
- [SDK Repository Layout](#sdk-repository-layout)
- [Project Structure and Module Organization](#project-structure-and-module-organization)
- [Build System Overview](#build-system-overview)
- [CMake Build Details](#cmake-build-details)
- [Configuration Model (Kconfig)](#configuration-model-kconfig)
- [Build Workflows](#build-workflows)
- [Build Outputs and Artifacts](#build-outputs-and-artifacts)
- [Image Generation](#image-generation)
- [Flashing](#flashing)
- [Where to Run Commands](#where-to-run-commands)
- [Applications (Modify or Create)](#applications-modify-or-create)
- [Command Reference](#command-reference)
- [Troubleshooting](#troubleshooting)
- [Related References](#related-references)

## Scope and Audience

Use this reference if you need to:
- Understand how the SDK is organized and built.
- Configure SDK or application features.
- Build SDK packages and applications with the CLI.
- Generate images and flash devices.
- Troubleshoot build, image generation, or flashing issues.

Before using this reference, complete the setup and installation steps:
- CLI setup: [Setup and Install SDK using CLI](./Setup_and_Install_SDK_using_CLI.md)

## Supported Platforms and Boards

### Boards
- **Astra Machina Micro (SR110_RDK)** 
- **SL2610_RDK** 

### Platform Differences (at a glance)
| Feature | SR110 | SL2610 |
| --- | --- | --- |
| MCU core | Cortex-M55 | Cortex-M52 |
| NPU | Ethos-U55 | None |
| Flash media | External XSPI | eMMC (via USB boot tool) |
| Image generation | SRSDK image generator | Image generator + USB boot tool |
| Debug | OpenOCD + probe | Not supported in this SDK flow |
| Toolchains | GCC, AC6, LLVM (SR110 only) | GCC |

### Platform Guides
- SR110 platform and hardware: [SR110 Platform Guide](./SR110/SR110_platform_Guide.md)
- SL2610 platform and hardware: [SL2610 Platform Guide](./SL2610/SL2610_Platform_Guide.md)

## Host Requirements

### Supported Host OS
- Windows (x64)
- Linux (Ubuntu 22.04)
- macOS (Apple Silicon & Intel)

### Required Tools
| Tool | Version Guidance | Notes |
| --- | --- | --- |
| CMake | 4.1.2 | Build system generator |
| Ninja | 1.13.1 | Build executor |
| Python | 3.13.x | Required for configuration tools and image generation |
| OpenOCD | Host package or xPack | Required for SR110 flashing |

### Toolchains
- **GCC 13.2.1** (all platforms)
- **Arm Compiler 6.19** (AC6)
- **LLVM Clang 21.x** (SR110 only; requires GCC sysroot)

Use only **one toolchain per build**. Do not mix toolchains across SDK and application builds.

## Environment Setup 
Use these for host setup and tool installation:
- [Setup and Install SDK using CLI](./Setup_and_Install_SDK_using_CLI.md)
- [Build environment guides](./build_env/) (OS + toolchain specific guides)


## SDK Repository Layout

Top-level directories:
- `arch/` CPU architecture support
- `board/` Board-specific support files
- `bootloader/` Bootloader sources
- `configs/` SDK defconfigs (bootloader, tflite, default package)
- `drivers/` Hardware drivers
- `examples/` Sample applications and example build system
- `kconfig/` SDK-level configuration entry
- `os/` RTOS support (FreeRTOS and related)
- `soc/` SoC-specific sources
- `tools/` Build, image, flashing, and utility tools
- `utilities/` Common utilities
- `third_party/` External dependencies

### Examples Layout (in-tree)
```
examples/
|-- <BOARD>/
|   |-- configs/                 # Application defconfigs
|   |-- kconfig                  # Example Kconfig
|   `-- <example sources>
|-- build/                       # Example build artifacts
|-- install/<BOARD>/             # Installed SDK package
|-- out/                         # Example output binaries and images
`-- Makefile                     # Examples build entry
```

Out-of-tree examples are supported. **Out-of-tree** means your application code lives in a separate directory from the SDK, keeping the SDK read-only and version-controlled independently. Point `SRSDK_DIR` to the SDK root `<sdk-root>` and keep your example repo separate.

## Project Structure and Module Organization

The SDK is organized as functional modules. Each module typically contains its own `CMakeLists.txt` and, when configurable, a `Kconfig` file. The root `CMakeLists.txt` includes major subsystems, and each subsystem adds its sources and include paths.

### Root-Level Build Entry
- `CMakeLists.txt` in the SDK root drives inclusion of core subsystems (bootloader, firmware, tflite, drivers, OS).
- `tools/cmake/flags.cmake` centralizes compiler flags and warnings.
- `tools/cmake/` contains SDK CMake modules; 
- `examples/tools/cmake/` contains example-specific CMake modules.

### Module-Level Structure
- Each module uses `target_sources()` to add sources and `target_include_directories()` to expose headers.
- Modules can be conditionally included based on Kconfig selections.

### Kconfig Hierarchy
- Top-level `kconfig` sources module Kconfig files (e.g., `drivers/Kconfig`, `soc/*/Kconfig`).
- Selecting options in Kconfig controls which modules and features are built.

### Where to Look for Specific Pieces
- **SoC configuration**: `soc/`
- **Board configuration**: `board/`
- **Drivers**: `drivers/`
- **OS abstraction**: `os/`
- **Bootloader**: `bootloader/`
- **TFLite Micro**: `AI/` and `third_party/`
- **Tools and scripts**: `tools/`
- **Example inclusion logic**: `examples/<BOARD>/CMakeLists.txt` (uses app `kconfig` symbols)

## Build System Overview

The SDK supports two primary build entry points:
- **SDK root (`<sdk-root>`)**: build the SDK package, bootloader, or TFLite Micro libraries.
- **Examples folder (`<sdk-root>/examples`)**: build applications, either app-only or combined SDK + app.

Out-of-tree builds are supported: keep your examples in a separate folder and set `SRSDK_DIR` to `<sdk-root>`.

The build system is layered:
- **Make**: orchestration and entry points (`Makefile` in SDK root and `examples/`).
- **Kconfig**: configuration UI and `.config` generation.
- **CMake**: build generation.
- **Ninja**: actual compilation and linking.

### Key Build Concepts
- **Defconfig**: Minimal config snapshot. Applied to produce `.config`.
- **.config**: Full configuration file, used to generate `config.h` and build flags.
- **config.h**: Generated header consumed by C/C++ sources.
- **SDK package**: Installed libraries and headers used by application builds.
- **SDK package installation**: Produces a reusable SDK package (headers, libraries, toolchain CMake files, and `config.h`) that application builds link against.

## CMake Build Details

This section describes the CMake flow at a practical level.

### SDK Root CMake (core SDK builds)
- Reads `build/config.h` to load `CONFIG_*` settings (generated from Kconfig).
- Uses those settings to select linker scripts and build flags.
- Builds the SDK libraries and packages them for reuse.
- Exports a CMake package under `examples/install/<BOARD>/lib/cmake/SynapticsSDK/`.

### Examples CMake (application builds)
- Builds app targets named `${CONFIG_PROJECT}_${CONFIG_BUILD_TYPE}` (for example `sr110_cm55_fw`).
- `BUILD=SRSDK` builds the SDK package and the app together.
- `BUILD=EXAMPLE` uses the installed SDK package.
- Example apps are included when their `kconfig` symbol is enabled.
- `USE_APP_CACHE=ON` (default) uses a small per‑app cache of SDK headers/libs for faster rebuilds.

## Configuration Model (Kconfig)

### Where Configuration Lives
- **SDK defconfigs**: `configs/<BOARD>/*_defconfig`
- **Example defconfigs**: `examples/<BOARD>/configs/*_defconfig`

### Configuration Flow
1. Apply a defconfig to generate `.config`.
2. (Optional) Modify using `menuconfig`.
3. Build generates `config.h` and uses it for compilation.

The Makefiles use Python helpers under `tools/scripts/kconfig/`:
- `defconfig.py` applies defconfigs.
- `menuconfig.py` launches the interactive menu.
- `genconfig.py` generates `config.h`.
- `savedefconfig.py` writes a minimal defconfig.

### Key Defconfig Fields
Common fields you will see in defconfigs:
- `CONFIG_PROJECT`: Project family (e.g., `sr110`, `sl2610`).
- `CONFIG_BUILD_TYPE`: Target type (e.g., `cm55_fw`, `cm52_fw`, `bootloader`, `tflite_micro`).
- `CONFIG_COMPILER`: Toolchain selection (`gcc`, `ac6`, `llvm`).
- `CONFIG_BOARD`: Board name (e.g., `sr110_rdk`, `sl2610_rdk`).
- `CONFIG_BOARD_REV`: Board revision (when applicable).

These values drive the build directory structure and output names.
`CONFIG_COMPILER` must match the active toolchain environment variables; use the same compiler for SDK and app builds.

### Build Modes
`CONFIG_BUILD_MODE` controls optimization and output directories:
- `release`: optimized output, minimal logging.
- `debug`: debug symbols and extra logging.

Build modes map to output folders: `release/` or `debug/`.

Note: Build types used in defconfigs are `cm55_fw`, `cm52_fw`, `bootloader`, and `tflite_micro`.

### SDK vs Example Menuconfig
- **SDK root menuconfig**: changes SDK-level options (drivers, compiler, features).
- **Examples menuconfig**: example-only options.
- **Advanced menuconfig**: combines SDK + example options and requires `SRSDK_DIR`.

## Build Workflows

### SDK Root Builds (from `<sdk-root>`)
Use SDK root for bootloader, tflite, or SDK package builds.

1. Apply a defconfig:
```
make default_config BOARD=<BOARD>
```

2. Build SDK package:
```
make astrasdk BOARD=<BOARD>
```

3. Build bootloader or tflite:
```
make <bootloader_defconfig> BOARD=<BOARD>
make build

make <tflite_defconfig> BOARD=<BOARD>
make build
```

### Example Builds (from `<sdk-root>/examples`)
Use examples to build applications.

Supported boards are defined in the Makefiles: currently `SR110_RDK` and `SL2610_RDK`.

**Path tips:** Use absolute paths for `SRSDK_DIR` and toolchain variables. On Windows, use escaped backslashes or forward slashes.

#### Combined SDK + App build
```
export SRSDK_DIR=<sdk-root>
make <app_defconfig> BOARD=<BOARD> BUILD=SRSDK
```

#### App-only build (uses installed SDK package)
```
export SRSDK_DIR=<sdk-root>
make build BOARD=<BOARD>
```

#### Build only the SDK package from examples
```
make astrasdk BOARD=<BOARD>
```
If you need to build the SDK package using the **default SDK defconfig** from the examples tree:
```
make default_config BOARD=<BOARD>
make astrasdk BOARD=<BOARD>
```

### BUILD Mode (examples only)
- `BUILD=EXAMPLE` (default): build the app using the installed SDK package.
- `BUILD=SRSDK`: build SDK package and the app in one flow (requires `SRSDK_DIR`).
- `BUILD=NONE`: apply defconfig only (no build).

**When to use:**
- Use **BUILD=SRSDK** for the first build, after SDK source changes, or when switching toolchains.
- Use **BUILD=EXAMPLE** for faster iteration when only app code changes and the SDK package is already installed.

### Build Multiple Applications with One SDK Package
If you want to build several applications against the same installed SDK:
```
cd <sdk-root>/examples
export SRSDK_DIR=<sdk-root>
make astrasdk BOARD=<BOARD>
make <app1>_defconfig BOARD=<BOARD>
make build BOARD=<BOARD>
make <app2>_defconfig BOARD=<BOARD>
make build BOARD=<BOARD>
```

### Create a Custom Defconfig (examples)
1. Start from an existing defconfig:
   ```
   make <app>_defconfig BOARD=<BOARD>
   ```
2. Open the configuration UI:
   ```
   make advanced_menuconfig BOARD=<BOARD>
   ```
3. Save your new defconfig under `<BOARD>/configs/` (for example `my_custom_app_defconfig`).
   ```
   make savedefconfig OUT=my_custom_app BOARD=<BOARD>
   ```
   This writes `examples/<BOARD>/configs/my_custom_app_defconfig`.

The new defconfig can then be used like any other:
```
make my_custom_app_defconfig BOARD=<BOARD> BUILD=SRSDK
```

## Build Outputs and Artifacts

### SDK Root Outputs
- `build/` contains build artifacts and `config.h` (CMake build tree is `build/<project>/<compiler>/`).
- `out/<target>/<mode>/` contains SDK root binaries:
  - GCC/LLVM: `<target>.elf` and `<target>.bin`
  - AC6: `<target>.axf` and `<target>.bin`
- `prebuilt/<PROJECT>/tflite_micro/<mode>/` contains TFLM libs when building `tflite_micro`.

### Example Outputs
- `examples/out/<target>/<mode>/` contains application binaries.
  - GCC/LLVM: `<target>.elf` and `<target>.bin`
  - AC6: `<target>.axf` and `<target>.bin`
- `examples/build/<project>/<compiler>/` contains the example CMake build tree.
- `examples/build/<project>/<compiler>/srsdk_build/.cache/` is the app-local SDK cache (auto-managed; refreshed when SDK content changes). Disable with `USE_APP_CACHE=OFF` if needed.
- `examples/install/<BOARD>/` contains the installed SDK package:
  - `include/` SDK headers
  - `lib/` SDK libraries
  - `config.h`
  - `tools/cmake/` SDK CMake toolchain modules used by the app build

The installed package also provides CMake package files under:
```
examples/install/<BOARD>/lib/cmake/SynapticsSDK/
```

### Generated Config Headers
- `build/config.h` in SDK root: generated from SDK `.config`.
- `examples/build/config.h`: example-local config overrides (if generated).
- `examples/build/example_config.h`: example-specific header for overrides.

Target naming:
- `<target>` is `${CONFIG_PROJECT}_${CONFIG_BUILD_TYPE}` (for example `sr110_cm55_fw` or `sl2610_cm52_fw`).
- `<mode>` is `release` or `debug`, based on `CONFIG_BUILD_MODE`.

## Image Generation

### SR110 Image Generation (SRSDK Image Generator)
**Tool:** `tools/srsdk_image_generator/srsdk_image_generator.py`

**Required:** Python 3.13 venv active and `tools/srsdk_image_generator/requirements.txt` installed.

**Typical command (from `<sdk-root>/tools/srsdk_image_generator`):**
```
cd <sdk-root>/tools/srsdk_image_generator
python srsdk_image_generator.py \
  -B0 \
  -flash_image \
  -sdk_secured \
  -spk "<sdk-root>/tools/srsdk_image_generator/B0_Input_examples/spk_rc4_1_0_secure_otpk.bin" \
  -apbl "<sdk-root>/tools/srsdk_image_generator/B0_Input_examples/sr100_b0_bootloader_ver_0x012F_ASIC.axf" \
  -m55_image "<sdk-root>/examples/out/sr110_cm55_fw/release/sr110_cm55_fw.elf" \
  -flash_type "GD25LE128" \
  -flash_freq "67"
```

**Output (example):**
```
<sdk-root>/tools/srsdk_image_generator/Output/B0_Flash/B0_flash_full_image_GD25LE128_67Mhz_secured.bin
```

Key flags:
- `-flash_image` or `-host_image`: choose flash or host image output.
- `-sdk_secured` / `-sdk_non_secured`: SDK security mode.
- `-model_secured` / `-model_non_secured`: model security mode.
- `-spk`, `-apbl`, `-m55_image`: required inputs for SR110.
- `-flash_type`, `-flash_freq`: flash configuration (types: `GD25LE128`, `W25Q128`, `MX25U128`; frequencies: `34`, `67`, `100`, `134` MHz).
- `-m4_image`, `-npu_c_image`, `-model`: additional payloads for multi-core or model flows.
- `-single_slot`: generate a single-slot image layout.
- `-json_attr` / `-parse_attr`: use JSON-based flash attributes.
- `-Q4`: flash 4-bit support (1/0).

### SL2610 Image Generation (Image Generator)
**Tool:** `tools/image_gen/image_generator.py`

**Required:** Python 3.13 venv active.

**Recommended entry:**
```
cd <sdk-root>/examples
make imagegen
```

This runs a two-step pipeline:
- `tools/image_gen/image_generator.py` to generate raw images using `tools/image_gen/inp.json`.
- `tools/scripts/image/build_preboot_mcu.py` to package sub-images for USB boot and eMMC workflows.

Final outputs:
- `examples/out/image/intermediate/sysmgr.subimg`
- `examples/out/image/eMMCimg/`
- `examples/out/image/usb_boot/`

Intermediate outputs from image generation:
- `examples/out/nexus_bin/`
- `examples/out/nexus_loadable/`

If you need to change inputs, update `tools/image_gen/inp.json`. Paths in this file are relative to `examples/out/`.
The preboot packaging step uses signing keys under `tools/signingKeys/<soc>/` and SPK assets for USB boot images.

If you see permission errors on Linux/macOS:
```
cd <sdk-root>/examples
chmod +x tools/scripts/image/bin/gen*
```

## Flashing

### SR110 (OpenOCD Flash Script)
**Tool:** `tools/openocd/scripts/flash_xspi_tcl.py`
OpenOCD config files live under `tools/openocd/configs/`. For SR110, `sr110_m55.cfg` is the standard config.

**Example (from `<sdk-root>`):**
```
cd <sdk-root>
python tools/openocd/scripts/flash_xspi_tcl.py \
  --cfg_path tools/openocd/configs/sr110_m55.cfg \
  --image tools/srsdk_image_generator/Output/B0_Flash/B0_flash_full_image_GD25LE128_67Mhz_secured.bin \
  --erase-all
```

Key options:
- `--cfg_path` (required): OpenOCD config file
- `--image`: Binary image to program
- `--erase-all`: Full chip erase
- `--verify-only`: Verify contents without programming
- `--probe`: Select probe (default `cmsis-dap`, use `jlink` for J-Link)
- `--openocd`: Path to OpenOCD binary
- `--flash-offset`: Flash offset for programming
- `--file-offset`: Offset into image file
- `--erase-only`: Erase only, no programming
- `--log-level`: DEBUG/INFO/WARN/ERROR
- `--tcl_port`, `--tcl_host`: OpenOCD TCL interface settings
- Use `--probe jlink` (or `--probe cmsis-dap`) to select the adapter driver.

### SL2610 (USB Boot Tool)
**Tool:** `tools/usb_boot_python_tool/USB_BOOT_TOOL/usb_boot_tool.py`

**Required:** Python 3.13 venv active and `pyserial` installed.

**Enter USB boot mode first:**
- Press and hold **USB_BOOT**, then press **RESET**.
- Release **RESET**, then release **USB_BOOT**.

**Run System Manager (from `<sdk-root>/tools/usb_boot_python_tool/USB_BOOT_TOOL`):**
```
cd <sdk-root>/tools/usb_boot_python_tool/USB_BOOT_TOOL
python usb_boot_tool.py --op run-sm \
  --sm <sdk-root>/examples/out/image/intermediate/sysmgr.subimg \
  --spk <sdk-root>/examples/out/image/usb_boot/spk.bin \
  --keys <sdk-root>/examples/out/image/usb_boot/key.bin \
  --m52bl <sdk-root>/examples/out/image/usb_boot/m52bl.bin
```

**Full eMMC flashing:**
```
python usb_boot_tool.py --op emmc --img-dir <path-to-eMMCimg>
```

Notes:
- `--op emmc` requires a Yocto-generated `eMMCimg` folder with `emmc_part_list` and `emmc_image_list`.
- If `run-sm` fails because SM CDC is already running, power-cycle the board and retry.

## Where to Run Commands

Use the table below to choose the correct working directory.

| Command or Task | Run From | Notes |
| --- | --- | --- |
| `make default_config BOARD=<BOARD>` | `<sdk-root>` | Applies SDK default defconfig |
| `make <bootloader_defconfig> BOARD=<BOARD>` | `<sdk-root>` | SDK bootloader build (follow with `make build`) |
| `make <tflite_defconfig> BOARD=<BOARD>` | `<sdk-root>` | SDK TFLite build (follow with `make build`) |
| `make astrasdk BOARD=<BOARD>` | `<sdk-root>` or `<sdk-root>/examples` | Builds and installs SDK package |
| `make <app_defconfig> BOARD=<BOARD> BUILD=SRSDK` | `<sdk-root>/examples` | Combined SDK + app build |
| `make build BOARD=<BOARD>` | `<sdk-root>/examples` | App-only build (requires `SRSDK_DIR`) |
| `make imagegen` | `<sdk-root>/examples` | SL2610 image generation pipeline |
| `srsdk_image_generator.py` | `<sdk-root>/tools/srsdk_image_generator` | SR110 image generation |
| `flash_xspi_tcl.py` | `<sdk-root>` | SR110 flashing (OpenOCD) |
| `usb_boot_tool.py` | `<sdk-root>/tools/usb_boot_python_tool/USB_BOOT_TOOL` | SL2610 USB boot flashing |

## Applications (Modify or Create)

### Modify an existing app
- Update sources under `examples/<BOARD>/<app>/`.
- If you change configuration options, re-run the app defconfig and rebuild:
```
cd <sdk-root>/examples
export SRSDK_DIR=<sdk-root>
make <app>_defconfig BOARD=<BOARD>
make build BOARD=<BOARD>
```

### Create a new app (minimal checklist)
1. Create `examples/<BOARD>/<app>/` with your sources under `src/`.
2. Add `examples/<BOARD>/<app>/kconfig` with a `config` symbol.
3. Add `examples/<BOARD>/configs/<app>_defconfig` and enable the app symbol (`CONFIG_<APP_SYMBOL>=y`).
4. Add `examples/<BOARD>/<app>/CMakeLists.txt` to add sources to `${TARGET}`.
5. Build from `examples/`:
```
cd <sdk-root>/examples
export SRSDK_DIR=<sdk-root>
make <app>_defconfig BOARD=<BOARD> BUILD=SRSDK
make build BOARD=<BOARD>
```

Minimal `kconfig`:
```
config <APP_SYMBOL>
    bool "Enable <app>"
    default y
```

Minimal `CMakeLists.txt` (pattern used by existing examples):
```cmake
if(CONFIG_<APP_SYMBOL>)
    if(${BUILD} STREQUAL "EXAMPLE")
        target_sources(${TARGET} PRIVATE
            ${CMAKE_CURRENT_LIST_DIR}/src/main.c
        )
        target_include_directories(${TARGET} PUBLIC
            ${CMAKE_CURRENT_LIST_DIR}
        )
    else()
        target_include_directories(sdk_includes INTERFACE
            $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
        )
    endif()
endif()
```

Notes:
- Use `cm55_<app>_defconfig` (SR110) or `cm52_<app>_defconfig` (SL2610).
- The board-level `examples/<BOARD>/CMakeLists.txt` auto-includes apps whose `kconfig` symbol is enabled.

Common mistakes:
- **Defconfig not found**: confirm `examples/<BOARD>/configs/` and `BOARD` match.
- **App not built**: ensure `CONFIG_<APP_SYMBOL>=y` is set in the app defconfig.
- **App directory skipped**: ensure the app folder has a `kconfig` file with a `config` symbol.
- **Missing sources**: ensure `CMakeLists.txt` lists all files under `src/`.
- **Wrong output target**: check `CONFIG_PROJECT` and `CONFIG_BUILD_TYPE`.

## Command Reference

### SDK Root (`<sdk-root>`)

| Command | Purpose | Result |
| --- | --- | --- |
| `make help` | Show available targets | Prints target list |
| `make list_defconfigs BOARD=<BOARD>` | List SDK defconfigs | Console list |
| `make default_config BOARD=<BOARD>` | Apply default SDK defconfig | `.config` updated |
| `make <name>_defconfig BOARD=<BOARD>` | Apply specific SDK defconfig | `.config` updated |
| `make menuconfig BOARD=<BOARD>` | Edit SDK config | `.config` updated |
| `make genconfig` | Generate `build/config.h` | `build/config.h` created |
| `make build` | Build current SDK target (bootloader/TFLM/firmware) | Use when you need the target binary only; outputs in `out/` |
| `make astrasdk BOARD=<BOARD>` | Build and install SDK package | Use when you need the SDK package for app builds; updates `examples/install/<BOARD>/` |
| `make savedefconfig OUT=name BOARD=<BOARD>` | Save minimal SDK defconfig | `configs/<BOARD>/name_defconfig` |
| `make clean` | Remove build artifacts | `build/` cleaned |
| `make cpp_check` | Run cppcheck | Console output |

### Examples (`<sdk-root>/examples`)

| Command | Purpose | Result |
| --- | --- | --- |
| `make help` | Show available targets | Prints target list |
| `make list_boards` | List valid boards | Console list |
| `make list_defconfigs BOARD=<BOARD>` | List example defconfigs | Console list |
| `make <app>_defconfig BOARD=<BOARD>` | Apply app defconfig | `.config` updated |
| `make <app>_defconfig BOARD=<BOARD> BUILD=SRSDK` | Combined SDK + app build | SDK installed + app built |
| `make <app>_defconfig BOARD=<BOARD> BUILD=NONE` | Apply defconfig only | `.config` updated |
| `make build BOARD=<BOARD>` | App-only build | Use for fast iteration with installed SDK; app binary in `out/` |
| `make build_sdk BOARD=<BOARD>` | Build SDK for example (combined flow) | Internal step used by `build_all`; builds SDK from source |
| `make build_advanced BOARD=<BOARD>` | Build app with advanced config | Internal step used by `build_all`; app binary in `out/` |
| `make build_all BOARD=<BOARD>` | SDK build + app build | Runs `build_sdk`, installs SDK, then `build_advanced` |
| `make menuconfig BOARD=<BOARD>` | Edit example-only config | `.config` updated |
| `make advanced_menuconfig BOARD=<BOARD>` | SDK + example config | `.config` updated (requires `SRSDK_DIR`) |
| `make advanced_genconfig` | Generate SDK + example config headers | `build/config.h` and `build/example_config.h` |
| `make genconfig` | Generate example config headers | `build/config.h` and `build/example_config.h` |
| `make default_config BOARD=<BOARD>` | Apply default SDK defconfig | `.config` updated |
| `make astrasdk BOARD=<BOARD>` | Build and install SDK package | `install/<BOARD>/` updated |
| `make custom_astrasdk` | Build and install SDK package using current example config | Use when you need an app-tailored SDK without building the app |
| `make imagegen` | Generate SL2610 images | Image outputs in `examples/out/image/` |
| `make apply_defconfig DEFCONFIG=<name> BOARD=<BOARD>` | Apply defconfig without building | `.config` updated |
| `make savedefconfig OUT=name BOARD=<BOARD>` | Save minimal defconfig | `examples/<BOARD>/configs/name_defconfig` |
| `make cpp_check` | Run cppcheck | Console output |
| `make clean` | Remove example build outputs | `build/` and `out/` cleaned |
| `make clean_package BOARD=<BOARD>` | Remove installed SDK package | `install/<BOARD>/` removed |
| `make all` | Alias for `clean_package` | `install/<BOARD>/` removed |

Notes:
- `build_sdk` and `build_advanced` are typically invoked by the defconfig flow and require `SRSDK_DIR` to be set.
- `BUILD=NONE` can be used to apply defconfig without building.

### Python Tools

| Tool | Location | Purpose |
| --- | --- | --- |
| `srsdk_image_generator.py` | `tools/srsdk_image_generator/` | SR110 image generation |
| `image_generator.py` | `tools/image_gen/` | SL2610 image generation |
| `build_preboot_mcu.py` | `examples/tools/scripts/image/` | SL2610 sub-image packaging |
| `flash_xspi_tcl.py` | `tools/openocd/scripts/` | SR110 flashing (OpenOCD) |
| `usb_boot_tool.py` | `tools/usb_boot_python_tool/USB_BOOT_TOOL/` | SL2610 USB boot flashing |
| `tools/scripts/kconfig/*` | `tools/scripts/kconfig/` | Defconfig/menuconfig/genconfig helpers |

### Tools Overview (Non-VS Code)
- `tools/openocd/`: OpenOCD configs and flashing scripts for SR110.
- `tools/usb_boot_python_tool/`: USB boot flashing tool for SL2610.
- `tools/image_gen/`: SL2610 image generator and `inp.json` configuration.
- `tools/srsdk_image_generator/`: SR110 image generator and input examples.
- `tools/Inference/`: Model conversion and inference helpers (see its README).
- `tools/JLink_Scripts/`: J-Link scripts for debugging/programming.
- `tools/Debug_IC_FW/`: Debug firmware support assets.
- `examples/tools/signingKeys/`: Signing keys used by SL2610 preboot packaging.
- `tools/SPK/`: SPK assets used by secure image flows.

Note: Some tools have their own dependency requirements (for example `tools/Inference/`). Follow the tool-specific README when using them.

## Troubleshooting

### Build and Configuration Issues
- **`SRSDK_DIR` not set (examples + SDK combined build fails)**
  - Fix: `export SRSDK_DIR=<sdk-root>` before `BUILD=SRSDK` or `advanced_menuconfig`.

- **Build and Deploy button disabled in VS Code**
  - Cause: Examples directory not imported or `SRSDK_DIR` not set.
  - Fix: import the SDK `examples/` folder, set `SRSDK_DIR` via **Import SDK**, then refresh the workspace (close and reopen VS Code).

- **App-only build fails because SDK package is missing**
  - Fix: build and install the SDK first with `make astrasdk BOARD=<BOARD>` (from SDK root or examples).

- **Toolchain not found**
  - Fix: ensure the correct toolchain env variable is set and the toolchain is on PATH.
  - Example: `GCC_TOOLCHAIN_13_2_1=/opt/gcc-arm-none-eabi/bin`.

- **LLVM build fails due to missing sysroot**
  - Fix: set `GCC_TOOLCHAIN_ROOT` to the GCC toolchain root.

- **`menuconfig` fails on Linux**
  - Fix: install `libncurses5-dev` and related packages per the Linux build_env guide.

- **`defconfig` not found**
  - Fix: verify the `BOARD` matches the directory under `configs/` or `examples/<BOARD>/configs/`.

- **`config.h` missing or stale**
  - Fix: run `make genconfig` (SDK root) or rebuild from examples to regenerate.

- **TFLite Micro library not found**
  - Fix: ensure `prebuilt/<PROJECT>/tflite_micro/<mode>/` contains the lib, or rebuild `tflite_micro`.
  - If NPU is enabled, TFLM library is required.

### Python / Image Generation Issues
- **Python version mismatch**
  - Fix: activate the SDK virtual environment and verify `python --version` is 3.13.x.

- **Missing Python packages**
  - Fix: `pip install -r tools/srsdk_image_generator/requirements.txt`.

- **Imagegen permission errors (Linux/macOS)**
  - Fix: `chmod +x tools/scripts/image/bin/gen*`.

### Flashing Issues
- **OpenOCD not found**
  - Fix: install OpenOCD and ensure it is in PATH.

- **OpenOCD cannot connect to target**
  - Fix: check cable, power, and probe selection (`--probe cmsis-dap` or `--probe jlink`).

- **WSL USB device not visible**
  - Fix: follow the [Astra MCU SDK - WSL User Guide](./Astra_MCU_SDK_WSL_User_Guide.md) for USB pass-through.

- **SL2610 USB boot fails**
  - Fix: re-enter USB boot mode (USB_BOOT + RESET), then retry.

### Build Output Issues
- **App builds but image generation fails**
  - Fix: confirm output `.elf` exists in `examples/out/` and update image generator input paths.

- **Example build does not pick up new SDK headers/libs**
  - Fix: clean the app cache (`rm -rf examples/build/<target>/<compiler>/srsdk_build/.cache`) or disable it by configuring with `USE_APP_CACHE=OFF`.

- **Link errors from mixed toolchains**
  - Fix: ensure SDK and app are both built with the same compiler (GCC, AC6, or LLVM).

## Related References

- SDK build and structure reference: [Astra MCU SDK User Guide](./Astra_MCU_SDK_User_Guide.md)
- CLI setup: [Setup and Install SDK using CLI](./Setup_and_Install_SDK_using_CLI.md)
- SR110 build and flash: [SR110 Build and Flash with CLI](./SR110/SR110_Build_and_Flash_with_CLI.md)
- SL2610 build and flash: [SL2610 Build and Flash with CLI](./SL2610/SL2610_Build_and_Flash_with_CLI.md)
