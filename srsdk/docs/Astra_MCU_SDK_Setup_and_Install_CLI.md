# Setup and Install SDK using CLI

This guide walks through the required steps to set up the Astra MCU SDK for command-line builds on Windows, Linux, or macOS.

Throughout this guide, `<sdk-root>` refers to the directory where you extracted or cloned the SDK.

## Table of Contents
- [Prerequisites](#prerequisites)
- [Get the SDK](#get-the-sdk)
- [Install the toolchain (choose one)](#install-the-toolchain-choose-one)
- [Install Python tools (for image generation and tooling)](#install-python-tools-for-image-generation-and-tooling)
- [Linux USB/serial permissions (recommended)](#linux-usbserial-permissions-recommended)
- [Set required environment variables](#set-required-environment-variables)
- [Verify the environment](#verify-the-environment)
- [Next: Build an example](#next-build-an-example)

## Prerequisites

- Windows users are strongly encouraged to use WSL2 with Ubuntu 22.04. See [Astra MCU SDK - WSL User Guide](./Astra_MCU_SDK_WSL_User_Guide.md).

## Get the SDK

- Extract or clone the SDK to a local directory (for example, `<sdk-root>`).
- Keep the SDK path short on Windows to avoid path length issues.

## Install the toolchain (choose one)

Select the host OS and toolchain guide that matches your environment:

**Windows**
- GCC: [Windows + GCC Compiler](./build_env/Astra_MCU_SDK_Windows_env_with_gcc.md)
- Arm Compiler 6: [Windows + ARM Compiler](./build_env/Astra_MCU_SDK_Windows_env_with_ARM_Compiler.md)
- LLVM Clang: [Windows + LLVM Clang](./build_env/Astra_MCU_SDK_Windows_env_with_LLVM_CLANG.md)

**Linux**
- GCC: [Linux + GCC Compiler](./build_env/Astra_MCU_SDK_Linux_env_with_gcc.md)
- Arm Compiler 6: [Linux + ARM Compiler](./build_env/Astra_MCU_SDK_Linux_env_with_ARM_Compiler.md)
- LLVM Clang: [Linux + LLVM Clang](./build_env/Astra_MCU_SDK_Linux_env_with_LLVM_CLANG.md)

**macOS**
- GCC (Apple Silicon + Intel): [macOS + GCC](./build_env/Astra_MCU_SDK_Mac_env_with_gcc.md)
- LLVM Clang (Apple Silicon + Intel): [macOS + LLVM Clang](./build_env/Astra_MCU_SDK_Mac_env_with_LLVM_CLANG.md)

## Install Python tools (for image generation and tooling)

Create a local virtual environment and install the required packages:

If you are using pyenv, activate it **before** creating the virtual environment:

```bash
cd <sdk-root>
pyenv local 3.13.7
python --version
```

**Linux/macOS** (run these commands from `<sdk-root>`)
```bash
cd <sdk-root>
python3 -m venv .venv
source .venv/bin/activate
pip install -r tools/srsdk_image_generator/requirements.txt
pip install pyserial
```

**Windows (PowerShell)** (run these commands from `<sdk-root>`)
```powershell
cd <sdk-root>
python -m venv .venv
.\.venv\Scripts\Activate.ps1
pip install -r tools\srsdk_image_generator\requirements.txt
pip install pyserial
```

Activate the virtual environment later if needed:

```bash
# Linux/macOS
source .venv/bin/activate
```

```powershell
# Windows PowerShell
.\.venv\Scripts\Activate.ps1
```

Note: Keep the virtual environment active when running SDK commands.

Verify you are using the virtual environment Python:

```bash
which python
python --version
```

Expected: `which python` points to `<sdk-root>/.venv/...` and `python --version` shows 3.13.x.

## Linux USB/serial permissions (recommended)

On Linux, add your user to the `dialout` group so the flashing tools can access USB CDC/UART devices:

```bash
sudo usermod -aG dialout $USER
```

Log out and log back in (or reboot) for the group change to take effect.

## Set required environment variables

Set the SDK root and toolchain paths in your shell.

**Linux/macOS**

```bash
export SRSDK_DIR=<sdk-root>
```

Note: `SRSDK_DIR` must be set in any shell where you build from your project folder (e.g. `<sdk-root>/examples/<project_category>/<project>`), including app-only builds.

Choose **one** toolchain variable (do not set multiple toolchains at the same time):

```bash
# GCC
export GCC_TOOLCHAIN_13_2_1=/path/to/gcc-arm-none-eabi/bin

# Arm Compiler 6
export AC6_TOOLCHAIN_6_19_0=/path/to/armclang/bin

# LLVM Clang
export LLVM_TOOLCHAIN_ROOT=/path/to/llvm/bin
export GCC_TOOLCHAIN_ROOT=/path/to/gcc-arm-none-eabi
```
Note: LLVM builds require `GCC_TOOLCHAIN_ROOT` for the GCC sysroot (ARM system headers) and the C++ standard library (libstdc++).

**Windows PowerShell**

```powershell
$env:SRSDK_DIR = "<sdk-root>"

# GCC
$env:GCC_TOOLCHAIN_13_2_1 = "C:\path\to\gcc-arm-none-eabi\bin"

# Arm Compiler 6
$env:AC6_TOOLCHAIN_6_19_0 = "C:\path\to\armclang\bin"

# LLVM Clang
$env:LLVM_TOOLCHAIN_ROOT = "C:\path\to\llvm\bin"
$env:GCC_TOOLCHAIN_ROOT = "C:\path\to\gcc-arm-none-eabi"
```
Note: LLVM builds require `GCC_TOOLCHAIN_ROOT` for the GCC sysroot (ARM system headers) and the C++ standard library (libstdc++).

To persist these variables, add them to your shell profile (Linux/macOS) or your PowerShell profile / System Environment Variables (Windows).

## Verify the environment

Run these to confirm tools are reachable:

```bash
cmake --version
ninja --version
make --version
python --version
openocd --version
```

Expected versions (or newer):
- CMake 4.1.2
- Ninja 1.13.1
- GNU Make 4.x
- Python 3.13.x
- OpenOCD 0.12.x (distribution package versions may vary)

Verify your selected toolchain is on PATH:
- GCC: `arm-none-eabi-gcc --version`
- Arm Compiler 6: `armclang --version`
- LLVM Clang: `clang --version`

## Next: Build an example

For step-by-step command-line build flows, see:
- [SR110 Build and Flash with CLI](./SR110/SR110_Build_and_Flash_with_CLI.md)
- [SL2610 Build and Flash with CLI](./SL2610/SL2610_Build_and_Flash_with_CLI.md)
