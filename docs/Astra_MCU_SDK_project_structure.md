# Astra MCU SDK Project Structure

## Table of Contents

- [Build System](#build-system)
- [Directory Structure](#directory-structure)
- [Build Project](#build-project)
- [Astra MCU SDK Project Structure](#Astra-MCU_SDK-project-structure)
  - [Root-Level CMake](#root-level-cmake)
  - [Module-Level CMake Structure](#module-level-cmake-structure)
  - [Configuration System: Kconfig](#configuration-system-kconfig)
  - [Conditional Compilation](#conditional-compilation)

---

## Build System

The Astra MCU SDK build system is based on **CMake** and uses a layered, modular architecture for flexibility and scalability. Each module and submodule is controlled using its own `CMakeLists.txt` file and, where applicable, its own `Kconfig` configuration file.

---

## Directory Structure

The Astra MCU SDK directory structure is organized as follows:

- **AI, arch, drivers, examples, utilities, etc.**: Each is a top-level functional category.
- **soc/**: Contains SoC-specific configurations, headers, and source code for different cores.
- **bootloader/**: Contains bootloader implementations.
- **os/**: Contains OS abstraction (e.g., FreeRTOS).
- **third_party/**: External dependencies (e.g., CMSIS).
- **tools/**: Contains CMake scripts, Kconfig utilities, VS Code extensions, and tools like SynaToolkit for application control.

> See full tree structure in [Astra MCU SDK Tree Structure](./Astra_MCU_SDK_Tree_Structure.md)

---

## Build Project

For instructions on building the SDK, refer to the [Astra MCU SDK Build System](./Astra_MCU_SDK_Build_System.md).

---

## Astra MCU SDK Project Structure

The Astra MCU SDK project uses a hierarchical and modular build setup managed via CMake and Kconfig.

---

### Root-Level CMake

The root `CMakeLists.txt` acts as the main entry point and controls inclusion of major sub-projects:

- `cm55_fw`: Main firmware
- `bootloader`: Bootloader implementation
- `tflite_micro`: tflite library components

Compiler options, optimization flags, and warnings are managed centrally in:
```
tools/cmake/flags.cmake
```
---

### Module-Level CMake Structure

Each module includes its own `CMakeLists.txt`, where it can:

- Add sources using `target_sources`
- Include headers using `target_include_directories`
- Add dependencies or link libraries

#### Example: Adding Source Files

```cmake
target_sources(${TARGET} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/your_source_file.c
)

target_include_directories(${TARGET} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)
```

#### Adding a New Module

To include a new module in the build:

```cmake
add_subdirectory(path/to/your_module)
```

This ensures the module's targets and configuration are picked up during the build.

---

### Configuration System: Kconfig

Astra MCU SDK uses the Kconfig system for modular project configuration. Each module includes its own Kconfig file, and the structure is hierarchical, with top-level configuration files sourcing module-specific Kconfigs as needed.

#### Example Kconfig Tree

```
drivers/
├── aon/
│   └── Kconfig
└── Kconfig
```

#### Adding a New Kconfig File

1. Create a `Kconfig` file in your module directory.
2. In the parent Kconfig file (e.g., `components.kconfig`), add:

```kconfig
source "path/to/your/module/Kconfig"
```

3. Run `menuconfig` or use a predefined `defconfig` file during the build.

---

### Conditional Compilation

The build process is managed by CMake, which includes only the necessary modules based on the selected Kconfig options. This approach ensures that only relevant source files are compiled, while unused modules are excluded from the final build.



