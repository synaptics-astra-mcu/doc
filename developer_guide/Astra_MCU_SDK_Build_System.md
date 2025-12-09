# Astra MCU SDK Build System (Reference)
## Contents
- [Requirements](#requirements)
- [Build system](#project-build-system)
- [Building via Native CLI](#building-via-native-cli-reference)
  - [Configuration](#configuration)
    - [SDK root configuration](#sdk-root-configuration)
    - [Examples configuration](#examples-configuration)
  - [Build Astra MCU SDK package](#build-astra-mcu-sdk-package)
  - [Build example application (examples/)](#build-example-application-examples)
  - [Build TFlite and bootloader](#build-tflite-and-bootloader)
  - [Clean the build](#clean-the-build)
  - [Help](#help)
- [Activate Python Virtual Environment](#activate-python-virtual-environment)
  - [Binary generation](#binary-generation)
    - [Generate Flash Binary](#generate-flash-binary)
    - [Generate Host Binary](#generate-host-binary)
    - [Arguments Description](#arguments-description)
  - [Flashing via CLI (OpenOCD)](#flashing-via-cli-openocd)
- [VS Code extension (reference)](#vs-code-extension-reference)
- [Targets reference (quick glance)](#targets-reference-quick-glance)

Scope: This page is a quick reference for the build tooling (Kconfig, CMake, Ninja), targets, and key variables. For step‑by‑step workflows (setup, build, image generation, flashing, and debugging), see the [Astra MCU SDK User Guide](../quickstart/Astra_MCU_SDK_User_Guide.md).

---

## Requirements

Please ensure that your system was set up according to one of the installations listed below
- Astra MCU SDK setup for Windows with ARM Compiler: [Setup_for_Astra_MCU_SDK_in_Windows_for_ARM_Compiler](./build_env/Astra_MCU_SDK_Windows_env_with_ARM_Compiler.md)
- Astra MCU SDK setup for Windows with GCC Compiler: [Setup_for_Astra_MCU_SDK_in_Windows_for_GCC](./build_env/Astra_MCU_SDK_Windows_env_with_gcc.md)
- Astra MCU SDK setup for Linux with ARM Compiler : [Setup_for_Astra_MCU_SDK_in_Linux_for_ARM_Compiler](./build_env/Astra_MCU_SDK_Linux_env_with_ARM_Compiler.md)
- Astra MCU SDK setup for Linux with GCC Compiler: [Setup_for_Astra_MCU_SDK_in_Linux_for_GCC](./build_env/Astra_MCU_SDK_Linux_env_with_gcc.md)
- Astra MCU SDK setup for macOS ARM64 with GCC Compiler: [Setup_for_Astra_MCU_SDK_in_macOS_ARM64_for_GCC_Compiler](./build_env/Astra_MCU_SDK_Mac_Arm64_env_with_gcc.md)
- Astra MCU SDK setup for macOS x86_64 with GCC Compiler: [Setup_for_Astra_MCU_SDK_in_macOS_x86_64_for_GCC](./build_env/Astra_MCU_SDK_Mac_x86_64_env_with_gcc.md)
- Astra MCU SDK VS Code Extension installation and setup: [Astra MCU SDK VS Code Extension User Guide](./Astra_MCU_SDK_VSCode_Extension_Userguide.rst)
- Astra MCU SDK Install and Setup Python: [Astra MCU SDK Install and Setup Python](./Astra_MCU_SDK_Install_and_Setup.md)

---

## Project build system
- Kconfig drives configuration; CMake + Ninja perform the builds.
- Two modes:
  - `BUILD=SRSDK`: build and install the SDK package (headers, libraries, toolchain cmake files, config header)
  - `BUILD=EXAMPLE`: build only the application using the installed SDK

Where to run
- SDK root (this repository root):
  - `make default_config BOARD=<BOARD>` then `make astrasdk BOARD=<BOARD>`
  - Installs SDK package to `<sdk-root>/examples/install/<BOARD>`
  - Also used for bootloader and TFLite builds
- examples/ (out‑of‑tree application repository/folder):
  - `SRSDK_DIR=<sdk-root>` must be set for combined builds and SDK install
  - `make <app_defconfig> BOARD=<BOARD> BUILD=SRSDK` (combined SDK + app)
  - `make build BOARD=<BOARD>` (app‑only rebuild)
  - Installs SDK package to `<examples-root>/install/<BOARD>` if you run `make astrasdk BOARD=<BOARD>` in examples

---

## Building via Native CLI (reference)

### Configuration

#### SDK root configuration
- Apply a default SDK configuration:
  - `make default_config BOARD=<BOARD>` → loads the default SDK package configuration.
- Apply other SDK/root presets:
  - Bootloader: `make cm55_sr110_bootloader_defconfig BOARD=SR110_RDK`
  - TFLite: `make cm55_tflite_micro_defconfig BOARD=SR110_RDK`
- Interactive edit (SDK root):
  - `make menuconfig BOARD=<BOARD>` → opens the Kconfig UI and writes selections to `.config` in the SDK root.
- List available defconfigs:
  - `make list_defconfigs BOARD=<BOARD>`
- Save a reusable defconfig (minimal):
  - `make savedefconfig OUT=my_config BOARD=<BOARD>` → saves under `configs/<BOARD>/my_config_defconfig`

#### Examples configuration
- What is “advanced” Kconfig?
  - The examples tree has two configuration entry points:
    - Examples‑local Kconfig (examples/kconfig): exposes application‑level options and a small overlay. Invoked by `make menuconfig` in the examples folder; does not require `SRSDK_DIR`.
    - Advanced Kconfig (SDK master Kconfig at `$SRSDK_DIR/kconfig`): exposes the full SDK option set (drivers, SoC features, compiler, build modes, etc.) in addition to the example overlay. Invoked by `make advanced_menuconfig` (or by using `EDIT=1` with `<app_defconfig>`). Requires `SRSDK_DIR` so the SDK source tree can be loaded.
  - Use advanced Kconfig when you want to change SDK‑level options that affect what gets built into the SDK package (e.g., enabling/disabling drivers, changing compiler, build modes), not just the app settings.

- Apply an application defconfig (examples/):
  - `make <app_defconfig> BOARD=<BOARD>` (or with `BUILD=SRSDK` to build SDK + app)
- One‑shot edit + build (advanced Kconfig first, then build):
  - `make <app_defconfig> BOARD=<BOARD> BUILD=SRSDK EDIT=1` (requires `SRSDK_DIR`)
- Edit only (advanced; no build):
  - `make advanced_menuconfig BOARD=<BOARD>` (requires `SRSDK_DIR`) → opens the combined SDK+example Kconfig UI; build later with `make build BOARD=<BOARD>`.
- Edit only (examples‑local):
  - `make menuconfig BOARD=<BOARD>` (no `SRSDK_DIR` required) → edits examples tree Kconfig and updates `.config`.
- List available application defconfigs:
  - `make list_defconfigs BOARD=<BOARD>`
- Save a reusable application defconfig (minimal):
  - one can save customised .config under `<BOARD>/configs/my_app_defconfig` and reuse.

### Build Astra MCU SDK package

To select desired default config for the target Board, the corresponding configuration is loaded into the .config file. This configuration determines which features, modules, and settings are included in the build.

Tip
- List available SDK/root defconfigs (default package, bootloader, TFLite):
  - `make list_defconfigs BOARD=<BOARD>`

Run the following to apply a default configuration:

```
make default_config BOARD=SR110_RDK
```

#### Build SDK package (root)
```
make default_config BOARD=SR110_RDK
make astrasdk BOARD=SR110_RDK
```
Installs to `<sdk-root>/examples/install/<BOARD>`.


### Build example application (examples/)
Tip
- List available application defconfigs in examples:
  - `cd examples`
  - `make list_defconfigs BOARD=<BOARD>`

```
# Combined build (SDK + App)
export SRSDK_DIR=<sdk-root>
make <app_defconfig> BOARD=SR110_RDK BUILD=SRSDK

# Rebuild app only (uses installed SDK)
make build BOARD=SR110_RDK
```

### Build TFlite and bootloader

From the SDK root, you can build the bootloader and TFLite libraries using defconfigs.

- Available defconfigs (may vary by board):
  - `cm55_sr110_bootloader_defconfig`
  - `cm55_tflite_micro_defconfig`

Commands (SDK root):
```
make <bootloader|tflite>_defconfig BOARD=SR110_RDK 
make build
```

- Outputs are placed under `out/<project>/<build_type>/` (e.g., `out/sr110_bootloader/release/sr110_bootloader.elf`).

### Clean the build

To clean the build

```makefile
make clean
```

### Help

The `make help` command displays a list of available build commands and their descriptions. It provides a quick reference for users to understand the available build targets, configuration options, and other utility commands within the build system.

Usage:

SDK root (this repository)

```
make help
```

Output:
```
Usage: make [target] [BOARD=value]

Common targets:
  help                   Show this help message
  list_defconfigs        Lists the defconfigs available to build the tflite and bootloader project
  menuconfig             Run menuconfig UI to edit configuration
  genconfig              Generate autoconf headers (requires build_config)
  build                  Configure and build the project (default goal)
  default_config         Apply default package defconfig
  astrasdk               Build and install standalone astrasdk
  savedefconfig          Save a minimal defconfig from current .config (use OUT=prefix)
  clean                  Remove build artifacts

Variables you can pass on the command line:
  BOARD=<board>          Board name

Examples (apply defconfig / build):
  make <bootloader/tflite_defconfig> BOARD=SR110_RDK Apply the chosen defconfig (e.g. bootloader or tflite)
  make build                                         Build the project (e.g. to build tflite or bootloader after applying defconfig)
  make default_config BOARD=SR110_RDK                Load default config to build SDK
  make astrasdk BOARD=SR110_RDK                      Build standalone SDK package

```
examples/ (out‑of‑tree application repository/folder)

```
Usage: make [target] [BOARD=<board_name>] [BUILD={EXAMPLE|SRSDK}] [EDIT=1]

Build Modes:
  BUILD=EXAMPLE        Default mode. Builds the example application using the installed SDK.
  BUILD=SRSDK          Builds the Astra MCU SDK package itself (requires SDK source via SRSDK_DIR).
Common Targets:
  <defconfig_name>     Build Example + Astra MCU SDK and Build Example using prebuilt package.
                       Example + SDK Build : make cm55_demo_sample_app_defconfig BOARD=SR110_RDK BUILD=SRSDK                    
					   Example Build : make cm55_demo_sample_app_defconfig BOARD=SR110_RDK
  build                Build the executable for the selected configuration.
                       Example: make build BOARD=SR110_RDK
  astrasdk             Build and install the default Astra MCU SDK package.
  custom_astrasdk      Build and install a custom Astra MCU SDK package based on current configs.
  list_defconfigs      List available defconfig files for the selected board.
                       Example: make list_defconfigs BOARD=SR110_RDK
  menuconfig           Run the standard interactive configuration menu.
  advanced_menuconfig  Run the advanced Kconfig menu (requires SRSDK_DIR).
  clean                Remove build output directories.
  clean_package        Remove installed SDK files.
  all                  Alias for clean_package.

Notes:
  - The environment variable SRSDK_DIR must be set when building SDK sources (BUILD=SRSDK or advanced targets).
  - To check available boards: VALID_BOARDS := SR110_RDK
  - Default BUILD mode is EXAMPLE.

```
---

### Activate python Virtual Environment

**Important:** Make sure to activate the Python virtual environment before running the image generation or flashing scripts.

Refer to [Astra_MCU_SDK_Install_and_Setup](./Astra_MCU_SDK_Install_and_Setup.md) for python installation.

**Windows PowerShell:**

```powershell
'C:\Users\<Username>\.sdk_venv\Scripts\Activate.ps1'
```

If you encounter the error `Activate.ps1 cannot be loaded because running scripts is disabled on this system`, run the below command first:

```powershell
powershell.exe -ExecutionPolicy Bypass
```

**Linux**

```bash
source /home/<username>/.sdk_venv/bin/activate
```

**macOS**

```bash
source /Users/<username>/.sdk_venv/bin/activate
```

### Binary generation

After building the application, the generated .elf/.axf executable must be converted into a downloadable .bin image for flashing or host-based download.
Use the following commands to generate Flash and Host images from your build output.
Tool: `tools/srsdk_image_generator/srsdk_image_generator.py`.

#### Generate Flash Binary

```bash
cd to <sdk-root/tools/srsdk_image_generator> directory, populate relative path to bin/axf/elf files in below command and run:
python srsdk_image_generator.py \
    -B0 \
    -flash_image \
    -sdk_secured \
    -spk "<relative_path_to_spk_bin>" \
    -apbl "<relative_path_to_apbl_axf>" \
    -m55_image "<relative_path_to_m55_elf>" \
    -model "<model_name_or_empty>" \
    -flash_type "<flash_part_number>" \
    -flash_freq "<flash_frequency_mhz>"
```

#### Generate Host Binary

```bash
cd to <sdk-root/tools/srsdk_image_generator> directory, give relative path to bin/axf/elf files in below command and run:
python srsdk_image_generator.py \
    -B0 \
    -host_image \
    -sdk_secured \
    -spk "<relative_path_to_spk_bin>" \
    -apbl "<relative_path_to_apbl_axf>" \
    -m55_image "<relative_path_to_m55_elf>"
```

---

#### **Arguments Description**

| Argument       | Description                                                      | Default Value                                                                           |
| -------------- | ---------------------------------------------------------------- | --------------------------------------------------------------------------------------- |
| `-B0`          | Target chip revision                                             | B0                                                                                      |
| `-flash_image` | Generate flash image (used for programming the external flash)   | —                                                                                       |
| `-host_image`  | Generate host image (used for USB/UART download)                 | —                                                                                       |
| `-sdk_secured` | Use secured SDK flow                                             | Enabled by default                                                                      |
| `-spk`         | Path to secure provisioning key `.bin` file (SPK)                | `tools/srsdk_image_generator/B0_Input_examples/spk_rc3_0_secure_otpk_0605.bin`          |
| `-apbl`        | Path to AP Bootloader `.axf` file                                | `tools/srsdk_image_generator/B0_Input_examples/sr100_b0_bootloader_ver_0x012F_ASIC.axf` |
| `-m55_image`   | Path to the generated application `.elf` / `.axf` from SDK build | Generated by SDK build flow (e.g., `out/<project>/<type>/*.elf`)                        |
| `-model`       | Optional model name to be added to image metadata                |                                                                            |
| `-flash_type`  | External flash part number                                       | `GD25LE128` *(Supported: GD25LE128, W25Q128, MX25U128)*                                 |
| `-flash_freq`  | SPI clock frequency for external flash (MHz)                     | `67` *(Supported: 34, 67, 100, 134)*                                                    |

---

> Note
The default SPK/APBL example files are located at:

```
tools/srsdk_image_generator/B0_Input_examples/
```

> These defaults will work for typical SR110\_RDK secure flashing.
> Only override them if using a custom key or bootloader.

---

### Flashing via CLI (OpenOCD) 

Use OpenOCD to program the generated flash image into external flash over SWD (CMSIS‑DAP or J‑Link).

Prerequisites
- Install OpenOCD and add exe to the environment variable `path`.
- Requires Python 3.13 , `pexpect` and `telnetlib`.
- And connect the board via the Debug IC (CMSIS‑DAP, J14) or a J‑Link.

Run below commands from SDK root.

1) Start OpenOCD server (in one terminal)
```
# CMSIS-DAP
cd <sdk-root>
openocd -f tools/openocd/configs/cmsis-dap.cfg -f tools/openocd/configs/target.cfg

# Or J-Link
cd <sdk-root>
openocd -f tools/openocd/configs/jlink.cfg -f tools/openocd/configs/target.cfg
```

2) Flash the full B0 image (in another terminal)
```
cd <sdk-root>
python .\tools\openocd\scripts\openocd_flash.py .\\out\\bin_files\\Output\\B0_Flash\\B0_flash_full_image_GD25LE128_67Mhz_secured.bin 0x0 0x0 1

```
Arguments
- `<image_flash.bin>`: path to the generated full flash image
- `<file_offset>`: usually `0x0`
- `<flash_offset>`: usually `0x0` (start of external flash)
- `<full_flash_erase>`: `1` to erase required region before flashing (`0` to skip)

Notes
- Python version: the helper script uses Python's telnetlib. Use Python 3.12 or earlier (Python 3.13 removed telnetlib).
- Ensure OpenOCD is running (telnet on port 4444). Adapter config must match your hardware.
- On Linux, you may need udev rules or run with appropriate permissions for HID/USB devices.

---

## VS Code extension (reference)
- Use the Astra MCU SDK VS Code extension for interactive build, image conversion, flashing, and debug.
- See: [Astra_MCU_SDK_VSCode_Extension_User_Guide](./Astra_MCU_SDK_VSCode_Extension_Userguide.rst) and the User Guide sections linked above.

---

## Targets reference (quick glance)

SDK root (this repository)
- `default_config`:
  - Description: Applies the default SDK package configuration (loads `configs/<BOARD>/default_package_defconfig` and writes `.config`).
  - Inputs: `BOARD` (required).
  - Outputs: Updated `.config` in the SDK root.
  - Example: `make default_config BOARD=SR110_RDK`
  - When to use: Prepare to build a standard SDK package with `make astrasdk BOARD=<BOARD>`.

- `<name>_defconfig` (root):
  - Description: Applies a specific SDK/root defconfig from `configs/<BOARD>/` (e.g., bootloader or TFLite presets) and writes `.config`.
  - Inputs: `BOARD` (required), target name (e.g., `cm55_sr110_bootloader_defconfig`).
  - Outputs: Updated `.config` in the SDK root.
  - Example: `make cm55_sr110_bootloader_defconfig BOARD=SR110_RDK`
  - When to use: Select the bootloader or TFLite configuration prior to `make build`.

- `astrasdk`:
  - Description: Builds and installs the SDK package using the current `.config` (package install includes headers, libraries, toolchain CMake files, and `config.h`).
  - Inputs: `BOARD` (required); relies on `.config` (use `default_config` or another defconfig first).
  - Outputs: SDK package under `<sdk-root>/examples/install/<BOARD>/`.
  - Example: `make astrasdk BOARD=SR110_RDK`
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
  - Example: `make list_defconfigs BOARD=SR110_RDK`
  - When to use: Discover available bootloader/TFLite or other SDK presets.

- `savedefconfig`:
  - Description: Saves the current `.config` as a minimal defconfig under `configs/<BOARD>/`.
  - Inputs: `BOARD` (required), `OUT=<name>` (optional; defaults to `defconfig`).
  - Outputs: `configs/<BOARD>/<name>_defconfig`.
  - Example: `make savedefconfig OUT=my_sdk BOARD=SR110_RDK`
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

examples/ (out‑of‑tree application repository/folder)
- `<app>_defconfig`:
  - Description: Applies the selected application defconfig from `<BOARD>/configs/`, updates `.config`, and then builds. If `BUILD=SRSDK` is set, it first builds and installs the SDK package, then builds the app; otherwise it builds the app using the installed SDK.
  - Inputs: `BOARD` (required), optional `BUILD=SRSDK`, optional `EDIT=1` (one‑shot advanced edit), `SRSDK_DIR` required when `BUILD=SRSDK`.
  - Outputs: Updated `.config`; binaries in `out/<target>/<build_type>/`; if `BUILD=SRSDK`, SDK package in `install/<BOARD>/`.
  - Example: `make cm55_demo_sample_app_defconfig BOARD=SR110_RDK BUILD=SRSDK`
  - When to use: Start from a known preset; build SDK + app in one step (combined) or just the app.

- `build`:
  - Description: Builds the application only against the already installed SDK package.
  - Inputs: `BOARD` (required); requires `install/<BOARD>/` SDK package.
  - Outputs: Binaries under `out/<target>/<build_type>/`.
  - Example: `make build BOARD=SR110_RDK`
  - When to use: Fast rebuilds when SDK hasn’t changed.

- `build_all`:
  - Description: Sequential convenience: builds the SDK package (SRSDK) and installs it, then builds the app (EXAMPLE).
  - Inputs: `BOARD` (required), `SRSDK_DIR` (required to build SDK from source).
  - Outputs: SDK package under `install/<BOARD>/`; app binaries under `out/<target>/<build_type>/`.
  - Example: `make build_all BOARD=SR110_RDK`
  - When to use: Clean “SDK then app” sequence in one command.

- `astrasdk`:
  - Description: Builds and installs the default SDK package from the examples tree (uses SDK default defconfig).
  - Inputs: `BOARD` (required), `SRSDK_DIR` (required).
  - Outputs: SDK package under `install/<BOARD>/`.
  - Example: `make astrasdk BOARD=SR110_RDK`
  - When to use: Prepare or refresh a default SDK package prior to app builds.

- `menuconfig`:
  - Description: Opens the examples‑local Kconfig UI and writes selections to `.config` in the examples folder (no `SRSDK_DIR` required).
  - Inputs: `BOARD` (for include roots), none required otherwise.
  - Outputs: Updated `.config`.
  - Example: `make menuconfig BOARD=SR110_RDK`
  - When to use: Tweak application/overlay options without changing SDK source options.

- `advanced_menuconfig`:
  - Description: Opens the advanced Kconfig UI sourced from the SDK (full option set + example overlay); no build is performed.
  - Inputs: `SRSDK_DIR` (required), `BOARD` (for context).
  - Outputs: Updated `.config` in the examples folder.
  - Example: `export SRSDK_DIR=/path/to/Astra_MCU_SDK && make advanced_menuconfig BOARD=SR110_RDK` 
			  `make custom_astrasdk BOARD=<BOARD>`
			  `make build BOARD=<BOARD>`
  - When to use: Change SDK‑level options (drivers, SoC features, compiler) before building; build later with `make build` or a combined flow.

- `genconfig` (advanced):
  - Description: Generates the examples config header `build/example_config.h` from `.config` without invoking a full build.
  - Inputs: `BOARD` (for include roots); existing `.config`.
  - Outputs: `build/example_config.h`.
  - Example: `make genconfig BOARD=SR110_RDK`
  - When to use: Rare; if you need headers generated from config without a compile (normal builds regenerate as needed).

- `clean` / `clean_package` / `all`:
  - Description: Cleans examples build artifacts and/or installed SDK package.
    - `clean`: removes `build/<target>/<compiler>` and related intermediate files.
    - `clean_package`: removes `install/<BOARD>` (installed SDK package).
    - `all`: alias for `clean_package`.
  - Inputs: `BOARD` (for `clean_package`).
  - Outputs: N/A (removes files/directories).
  - Example: `make clean BOARD=SR110_RDK`; `make clean_package BOARD=SR110_RDK`.
  - When to use: Reset build state; remove installed SDK to force a fresh install.

- `list_defconfigs` / `help`:
  - Description: Introspection helpers.
    - `list_defconfigs`: lists available application defconfigs under `<BOARD>/configs`.
    - `help`: prints usage, supported variables, and common targets.
  - Inputs: `BOARD` (for `list_defconfigs`).
  - Outputs: Console listing.
  - Example: `make list_defconfigs BOARD=SR110_RDK`
  - When to use: Discover available presets or commands.

Common variables
- `SRSDK_DIR`: Path to SDK root (required in examples for combined builds and SDK install).
