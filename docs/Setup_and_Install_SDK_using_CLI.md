# Setup and Install SDK using CLI

This guide walks through the required steps to set up the Astra MCU SDK for command-line builds on Windows, Linux, or macOS.

## 1) Get the SDK

- Extract the SDK package to a local directory (for example, `<sdk-root>`).
- Keep the SDK path short on Windows to avoid path length issues.

## 2) Install the toolchain (choose one)

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
- ARM64 + GCC: [macOS ARM64 + GCC](./build_env/Astra_MCU_SDK_Mac_Arm64_env_with_gcc.md)
- x86_64 + GCC: [macOS x86_64 + GCC](./build_env/Astra_MCU_SDK_Mac_x86_64_env_with_gcc.md)
- LLVM Clang: [macOS + LLVM Clang](./build_env/Astra_MCU_SDK_Mac_env_with_LLVM_CLANG.md)

## 3) Install Python tools (for image generation and tooling)

Use the provided scripts to install Python (if needed), create a virtual environment, and install required packages:

**Windows (PowerShell)**
```powershell
powershell.exe -ExecutionPolicy Bypass -File <sdk-root>\tools\vscode_extension\install_scripts\windows\install_tools.ps1 -InstallPath "<Path to Installation Directory>" python
```

**Linux**
```bash
sudo bash <sdk-root>/tools/vscode_extension/install_scripts/linux/install_tools.sh "<Path to Installation Directory>" python
```

**macOS**
```bash
sudo bash <sdk-root>/tools/vscode_extension/install_scripts/mac/install_tools.sh "<Path to Installation Directory>" python
```

Activate the virtual environment later if needed:

```bash
# Linux
source /home/<username>/.sdk_venv/bin/activate

# macOS
source /Users/<username>/.sdk_venv/bin/activate
```

```powershell
# Windows PowerShell
& 'C:\Users\<Username>\.sdk_venv\Scripts\Activate.ps1'
```

## 4) Set required environment variables

Set the SDK root and toolchain paths in your shell.

**Linux/macOS**

```bash
export SRSDK_DIR=<sdk-root>
```

Choose one toolchain variable:

```bash
# GCC
export GCC_TOOLCHAIN_13_2_1=/path/to/gcc-arm-none-eabi/bin

# Arm Compiler 6
export AC6_TOOLCHAIN_6_19_0=/path/to/armclang/bin

# LLVM Clang
export LLVM_TOOLCHAIN_ROOT=/path/to/llvm/bin
```

**Windows PowerShell**

```powershell
$env:SRSDK_DIR = "<sdk-root>"

# GCC
$env:GCC_TOOLCHAIN_13_2_1 = "C:\path\to\gcc-arm-none-eabi\bin"

# Arm Compiler 6
$env:AC6_TOOLCHAIN_6_19_0 = "C:\path\to\armclang\bin"

# LLVM Clang
$env:LLVM_TOOLCHAIN_ROOT = "C:\path\to\llvm\bin"
```

## 5) Verify the environment

Run these to confirm tools are reachable:

```bash
cmake --version
ninja --version
make --version
python --version
```

## Next: Build an example

For step-by-step command-line build flows, see:
- [SR110 Build and Flash with CLI](./SR110/SR110_Build_and_Flash_with_CLI.md)
