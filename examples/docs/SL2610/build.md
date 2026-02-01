# 🔧 Astra MCU SDK Examples - SL2610 Build Guide

Complete step-by-step guide for building Synaptics SDK (Astra MCU SDK) and example applications for SL2610 projects.

This guide covers all build workflows, from first-time setup to advanced configuration.

---

## 📁 Directory Layout Overview

```
examples/
├── build/                          # Intermediate CMake build files
│   └── <target>/<compiler>/
│       └── srsdk_build/.cache/     # App-local SDK cache (auto-managed)
├── install/                        # Installed Astra MCU SDK Package
│   └── <BOARD>/
│       ├── include/                # SDK headers
│       ├── lib/                    # SDK libraries (.a/.lib)
│       ├── prebuilt/               # prebuilt libraries (.a/.lib)
│       ├── config/                 # config.h and linker scripts
│       └── tools/                  # Toolchain cmake files
├── out/                            # Final application binaries & prebuilt libs
│   └── bin_files/                  # Application binary outputs
│   └── <target>/                   # Application build outputs
│       ├── release/                # Release binaries (.elf/.axf)
│       ├── debug/                  # Debug binaries
├── SL2610_RDK/                     # RDK-specific sample apps
│   ├── configs/                    # Application-specific defconfig files
│   │   └── <application_defconfig>
│   ├── drivers_sample_app/
│   ├── system_manager/
├── CMakeLists.txt                  # Main build configuration
├── Makefile                        # Build orchestration
├── kconfig                         # Example + Dependent configurations
├── docs/
│   └── SL2610/  
│       └── build.md                # This build guide
└── tools/
    └── cmake/
        └── subdirectories.cmake    # Lists example subdirectories to build
```

**Notes:**
- **App-local cache:** The build system uses `build/<target>/<compiler>/srsdk_build/.cache/` to automatically copy SDK headers and libraries for faster builds. The cache is refreshed only when SDK content changes, detected via hash-based change detection.
- **subdirectories.cmake:** Located at `examples/tools/cmake/subdirectories.cmake`, this file lists which example subdirectories to include in the build (e.g., `add_subdirectory(SL2610_RDK)`). It's included by CMakeLists.txt and allows you to easily control which examples are built. You can add or remove entries to customize the build scope.

## Prerequisites:

### Required tools

**Windows:**
- [Windows + GCC Compiler](../../../docs/build_env/Astra_MCU_SDK_Windows_env_with_gcc.md)
- [Windows + ARM Compiler](../../../docs/build_env/Astra_MCU_SDK_Windows_env_with_ARM_Compiler.md)
- [Windows + LLVM Clang](../../../docs/build_env/Astra_MCU_SDK_Windows_env_with_LLVM_CLANG.md)

**Linux:**
- [Linux + GCC Compiler](../../../docs/build_env/Astra_MCU_SDK_Linux_env_with_gcc.md)
- [Linux + ARM Compiler](../../../docs/build_env/Astra_MCU_SDK_Linux_env_with_ARM_Compiler.md)
- [Linux + LLVM Clang](../../../docs/build_env/Astra_MCU_SDK_Linux_env_with_LLVM_CLANG.md)

**macOS:**
- [macOS ARM64 + GCC](../../../docs/build_env/Astra_MCU_SDK_Mac_Arm64_env_with_gcc.md)
- [macOS x86_64 + GCC](../../../docs/build_env/Astra_MCU_SDK_Mac_x86_64_env_with_gcc.md)
- [macOS + LLVM Clang](../../../docs/build_env/Astra_MCU_SDK_Mac_env_with_LLVM_CLANG.md)

---

Set the base Astra MCU SDK directory path as an environment variable named **"SRSDK_DIR"**

**Linux/macOS:**
```bash
export SRSDK_DIR=/path/to/Astra MCU SDK
```

**Windows (PowerShell):**
```powershell
$env:SRSDK_DIR="C:\Users\admin\Downloads\Astra MCU SDK"
```

**VS Code:** Edit `.vscode/settings.json`:
```json
{
    "terminal.integrated.env.windows": {
        "SRSDK_DIR": "C:\\Users\\admin\\Downloads\\Astra MCU SDK"
    }
}
```

**Toolchain Environment Variables:**
```bash
export GCC_TOOLCHAIN_13_2_1=/path/to/gcc-arm-none-eabi
```

---

## 1. Building Application + SDK (combined) and Example Application

To build an **example application** along with its required SDK configuration:

Set the SDK root (required for combined builds):
```bash
export SRSDK_DIR=/path/to/Astra_MCU_SDK
```

```bash
make <application_defconfig> BOARD=<BOARD> BUILD=SRSDK
```

Example:
```bash
make cm55_demo_sample_app_defconfig BOARD=SL2610_RDK BUILD=SRSDK
```

This performs:
1. Apply the app defconfig (e.g., `cm55_demo_sample_app_defconfig`)
2. Generate configuration headers
3. Build the SDK with app‑specific configuration (BUILD=SRSDK)
4. Install the SDK package to `./install/<BOARD>/`
5. Build the example application
6. Place the final binary in `./out/<target>/<build_type>/`

Note:
- To reuse an existing SDK package, rebuild only the app via `make build BOARD=<BOARD>`.

To edit the configuration before building:

```bash
make <application_defconfig> BOARD=<BOARD> BUILD=SRSDK EDIT=1
```

**Example:**
```bash
make cm55_demo_sample_app_defconfig BOARD=SL2610_RDK BUILD=SRSDK EDIT=1
```

This target performs the following steps:

1. Applies the specified defconfig (e.g., `cm55_demo_sample_app_defconfig`)
2. Opens the interactive menuconfig editor
3. Saves your changes
4. Builds SDK and application with your custom configuration
---

## 2. Rebuilding Only the Example Application

If the SDK package is already installed, rebuild only the **example application**:

```bash
make build BOARD=<BOARD>
# or simply (default target is build)
make BOARD=<BOARD>
```

Example:
```bash
make build BOARD=SL2610_RDK
```

Notes
- Requires an installed SDK under `./install/<BOARD>/`. If missing, set `SRSDK_DIR` and run a combined build first.
- Rebuilds only the application sources using the previously loaded configs; faster than rebuilding the SDK.

---
## 3. Cleaning Build Outputs

**Remove intermediate build artifacts:**
```bash
make clean BOARD=<BOARD>
```
Removes `build/<target>/<compiler>/` directory

**Remove installed SDK package:**
```bash
make clean_package BOARD=<BOARD>
```
Removes `install/${BOARD}/` directory

**Remove both build and install directories:**
```bash
make clean all BOARD=<BOARD>
```
Complete cleanup of all build outputs
---

##  Summary of Useful Targets (examples)

| Target                         | Description                                                 |
|--------------------------------|------------------------------------------------------------ |
| `<app>_defconfig`              | Apply app-specific config and build SDK + example           |
| `list_defconfigs`              | Show available defconfig options                            |
| `menuconfig`                   | Run examples-local configuration editor                     |
| `advanced_menuconfig`          | Run advanced SDK+example config editor (requires SRSDK_DIR) |
| `genconfig`                    | Generate `build/example_config.h` from `.config`            |
| `build`                        | Build application using pre-built SDK                       |
| `astrasdk`                     | Build standalone Astra MCU SDK with default configuration   |
| `custom_astrasdk`              | Build Astra MCU SDK with current configuration              |
| `clean`                        | Remove build directory (intermediate build outputs)         |
| `clean_package`                | Remove installed SDK package (install directory)            |
| `clean all`                    | Remove both build directory and installed package           |

---

## Understanding Build Modes

The build system supports two modes controlled by the `BUILD` variable:

### BUILD=SRSDK (SDK Build Mode)
- Builds SDK libraries from source
- Installs to `install/${BOARD}/`
- Does **not** build example applications
- Used when SDK needs to be rebuilt

### BUILD=EXAMPLE (Application Build Mode - Default)
- Builds example applications
- Uses pre-built SDK from `install/${BOARD}/`
- Faster rebuilds
- Default mode for `make build`

**Key Behavior:**
- `make <app>_defconfig` → Builds SDK during defconfig step (if BUILD=SRSDK)
- `make build` → Always uses `BUILD=EXAMPLE`

---

## Troubleshooting

### Error: "SRSDK_DIR is not set"

**Cause:** Environment variable not configured

**Solution:**
```bash
# Linux/macOS
export SRSDK_DIR=/path/to/Astra MCU SDK

# Windows PowerShell
$env:SRSDK_DIR="C:\path\to\Astra MCU SDK"
```

### Error: "Toolchain not found"

**Cause:** Toolchain environment variable not set

**Solution:**
```bash
# For GCC
export GCC_TOOLCHAIN_13_2_1=/path/to/gcc-arm-none-eabi

# For AC6
export AC6_TOOLCHAIN_6_19_0=/path/to/armclang

# For LLVM
export LLVM_TOOLCHAIN_ROOT=/path/to/clang/bin
```

### Error: "SDK prebuilts not found"

**Cause:** SDK not built or installed

**Solution:**
```bash
make astrasdk BOARD=<BOARD>
```

### Error: "Linker script not found"

**Cause:** SDK installation incomplete

**Solution:**
```bash
# Verify linker scripts exist
ls install/SL2610_RDK/SL2610_RDK/config/*gcc_arm.ld  # For GCC
ls install/SL2610_RDK/SL2610_RDK/config/*.sct # For AC6
ls install/SL2610_RDK/SL2610_RDK/config/*llvm_arm.ld  # For LLVM

# If missing, rebuild SDK
make clean_package BOARD=<BOARD>
make astrasdk BOARD=<BOARD>
```

### Error: "No SDK libraries found"

**Cause:** SDK libraries not installed

**Solution:**
```bash
# Check if libraries exist
ls install/SL2610_RDK/lib/

# If empty, rebuild SDK
make astrasdk BOARD=<BOARD>
```

### Want to rebuild SDK from scratch

**Solution:**
```bash
make clean_package BOARD=<BOARD>  # Remove SDK installation
make astrasdk BOARD=<BOARD>  # Rebuild SDK
```

### Application fails to link

**Cause:** SDK and application built with different configurations

**Solution:**
```bash
# Rebuild both SDK and application
make clean all BOARD=<BOARD>
make <app>_defconfig BOARD=<BOARD>
```

---

## Advanced Topics

### Using Different Compilers

**Switch from GCC to AC6:**
1. Edit your defconfig or use menuconfig
2. SRSDK BUILD CONFIGURATION -> COMPILER SELECTION -> clear `gcc` and input `ac6`
3. SRSDK BUILD CONFIGURATION -> Toolchain SELECTION -> clear `GCC.13.2.1` and input `AC6.6.18.0`
3. Rebuild SDK and application

**Switch from GCC to LLVM:**
1. Edit your defconfig or use menuconfig
2. SRSDK BUILD CONFIGURATION -> COMPILER SELECTION -> clear `gcc` and input `LLVM`
3. SRSDK BUILD CONFIGURATION -> Toolchain SELECTION -> clear `GCC.13.2.1` and input `LLVM`
3. Rebuild SDK and application

### Creating Custom Defconfigs (examples)

```bash
# Step 1: Start with existing defconfig
make <application_defconfig> BOARD=<BOARD>

# Step 2: Customize via menuconfig
make advanced_menuconfig

# Step 3: Save as new defconfig "my_custom_app_defconfig" from .config under "<BOARD>/configs/" path

# Result: <BOARD>/configs/my_custom_app_defconfig
```

**Example:**
```bash
make sl2610_system_manager_rdk_defconfig BOARD=SL2610_RDK
make advanced_menuconfig
save a new defconfig from .config under "<BOARD>/configs/" path
# Result: <BOARD>/configs/my_custom_app_defconfig
```

### Cross-Compilation Tips

- Ensure toolchain paths are correct for your host OS
- Use absolute paths for `SRSDK_DIR` and toolchain variables
- On Windows, use forward slashes or escaped backslashes in paths

---

## Important Notes

> **Compiler Support Limitation:** The SL2610 SoC currently supports GCC-based builds only. Other compiler toolchains are not supported at this time.

> **Output Location:** The final executable is generated in `out/<target>/<build_type>/` and is ready for validation on the target hardware.

> **Static Libraries:** You can find the static library for each module (e.g., `libdrivers.a`) inside the `build/<target>/<compiler>/` directory.

> **SDK Changes:** When changes are made to Astra MCU SDK modules, the SDK package must be rebuilt and installed into the `install/` directory. This ensures that subsequent example builds use the updated libraries correctly.

> **Build Dependencies:** The build system automatically detects SDK changes via hash-based change detection and refreshes the application cache when needed.
