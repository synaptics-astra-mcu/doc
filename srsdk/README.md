# Astra MCU SDK Documentation

The Astra MCU SDK is the unified development platform for Synaptics MCUs, purpose-built for edge AI, vision, and audio applications. It standardizes workflows across devices and kits with both VS Code and command-line paths for setup, build, flash, and debug.

It combines optimized libraries, a production-grade build system, and integrated tooling so teams can move from evaluation to production quickly:

- Optimized AI stack with TensorFlow Lite Micro and CMSIS-NN accelerations.
- Kconfig + CMake + Ninja build system with reproducible defconfigs and fast incremental builds.
- Cross-platform toolchain support for GCC, Arm Compiler 6, and LLVM Clang on Windows, Linux, and macOS.
- Rich examples and out-of-tree workflows to keep applications cleanly separated from the SDK.
- Integrated tooling for image conversion, flashing, logging, and memory analysis.
- Optional VS Code development environment.

## Getting Started

### Step 1: Obtain the SDK
Extract or clone the SDK to a local directory (for example, `/path/to/Astra_MCU_SDK`). This folder is your SDK root, in the rest of this document it will be refered to as <sdk-root>.
- On Windows keep the path short (less than 100 characters) to avoid path length issues.
- **Note:** In order to clone the Astra MCU SDK repository you must first install Git LFS. For more details, see [https://git-lfs.com/](https://git-lfs.com/).

### Step 2: Install SDK Tools (VS Code or CLI)
- [Setup and Install SDK using VS Code](./docs/Astra_MCU_SDK_Setup_and_Install_VsCode.md)
- [Setup and Install SDK using CLI](./docs/Astra_MCU_SDK_Setup_and_Install_CLI.md)
- [Astra MCU SDK Docker Guide](./docs/Astra_MCU_SDK_Docker_User_Guide.md)

### Step 3: Build and Flash
#### SR110
- [SR110 Build and Flash with VSCode](./docs/SR110/SR110_Build_and_Flash_with_VSCode.md)
- [SR110 Build and Flash with CLI](./docs/SR110/SR110_Build_and_Flash_with_CLI.md)

#### SL2610 (Requires Linux)
- [SL2610 Build and Flash with VSCode](./docs/SL2610/SL2610_Build_and_Flash_with_VSCode.md)
- [SL2610 Build and Flash with CLI](./docs/SL2610/SL2610_Build_and_Flash_with_CLI.md)

### Step 4: Learn More
- [Astra MCU SDK User Guide](./docs/Astra_MCU_SDK_User_Guide.md)
- [VSCode Extension User Guide](./docs/Astra_MCU_SDK_VSCode_Extension_User_Guide.md)
- [Astra MCU SDK - WSL User Guide](./docs/Astra_MCU_SDK_WSL_User_Guide.md)

## Driver User Guide
- [Astra MCU SDK Driver Guide (HTML)](../api_doc/index.rst)

> *Note: To view the guide, open `index.html` in a web browser.*
