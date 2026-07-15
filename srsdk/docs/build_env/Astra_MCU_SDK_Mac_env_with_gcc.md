# Setup Guide for Development Environment on macOS for GCC

## Table of Contents
- [Install Basic Tools and Dependencies](#install-basic-tools-and-dependencies)
- [Install CMake](#install-cmake)
- [Install Ninja Build System](#install-ninja-build-system)
- [Install Python](#install-python)
- [Install OpenOCD](#install-openocd)
- [Install the Arm GNU GCC Compiler](#install-the-arm-gnu-gcc-compiler)

## Install Basic Tools and Dependencies

First, install [Homebrew](https://brew.sh/) if it's not already installed.

```bash
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```

Then install essential tools:

```bash
brew update
brew install git wget make zip unzip python
```

## Install CMake

Download CMake 4.1.2 for macOS.

```bash
curl -LO https://github.com/Kitware/CMake/releases/download/v4.1.2/cmake-4.1.2-macos-universal.tar.gz
```

Extract the archive:

```bash
tar -xzf cmake-4.1.2-macos-universal.tar.gz
```

Move CMake to a system directory:

```bash
sudo mv cmake-4.1.2-macos-universal /opt/cmake-4.1
```

Add CMake to PATH:

```bash
echo 'export PATH="/opt/cmake-4.1/CMake.app/Contents/bin:$PATH"' >> ~/.zshrc
source ~/.zshrc
```

Verify installation:

```bash
cmake --version
```

## Install Ninja Build System

Ninja is a small build system with a focus on speed, which CMake can utilize to manage builds.

Download Ninja v1.13.1 for macOS:

```bash
curl -LO https://github.com/ninja-build/ninja/releases/download/v1.13.1/ninja-mac.zip
```

Unzip the archive:

```bash
unzip ninja-mac.zip
```

Create the target directory and copy the binary:

```bash
sudo mkdir -p /opt/ninja/
sudo cp ninja /opt/ninja/
```

Make the binary executable:

```bash
sudo chmod a+x /opt/ninja/*
```

Add to PATH:

```bash
echo 'export PATH=$PATH:/opt/ninja' >> ~/.zshrc
source ~/.zshrc
```

Verify installation:

```bash
ninja --version
```

## Install Python

Python 3.13.x or newer is required to run configuration tools (menuconfig, kconfig) and to execute scripts used during SDK build and setup.

Choose **one** of the options below:

### Option A: System Python (if already 3.13.x+)
```bash
python3 --version
```

### Option B: pyenv (recommended if you need to install Python 3.13.x)
```bash
brew install pyenv
export PATH="$HOME/.pyenv/bin:$PATH"
eval "$(pyenv init -)"
eval "$(pyenv virtualenv-init -)"

pyenv install 3.13.7
pyenv local 3.13.7
python3 --version
```

Optional: make pyenv available in future shells (skip this if you prefer to enable pyenv manually per shell):

```bash
echo 'export PATH="$HOME/.pyenv/bin:$PATH"' >> ~/.zshrc
echo 'eval "$(pyenv init -)"' >> ~/.zshrc
echo 'eval "$(pyenv virtualenv-init -)"' >> ~/.zshrc
source ~/.zshrc
```

If you chose the optional lines above, restart your shell or run `source ~/.zshrc` to apply them.

## Install OpenOCD

```bash
brew install openocd
openocd --version
```

## Install the Arm GNU GCC Compiler

Check your architecture:

```bash
uname -m
```

Download Arm GNU Toolchain 13.2.rel1 for macOS:

```bash
# Apple Silicon (arm64)
curl -Lo gcc-arm-none-eabi.tar.xz https://developer.arm.com/-/media/Files/downloads/gnu/13.2.rel1/binrel/arm-gnu-toolchain-13.2.rel1-darwin-arm64-arm-none-eabi.tar.xz

# Intel (x86_64)
curl -Lo gcc-arm-none-eabi.tar.xz https://developer.arm.com/-/media/Files/downloads/gnu/13.2.rel1/binrel/arm-gnu-toolchain-13.2.rel1-darwin-x86_64-arm-none-eabi.tar.xz
```

Create a target directory for installation:

```bash
sudo mkdir -p /opt/gcc-arm-none-eabi
```

Extract the archive into the target directory:

```bash
sudo tar -xf gcc-arm-none-eabi.tar.xz --strip-components=1 -C /opt/gcc-arm-none-eabi
```

Environment variables for the Arm GNU Toolchain:

```bash
echo 'export PATH=$PATH:/opt/gcc-arm-none-eabi/bin' >> ~/.zshrc
echo 'export GCC_TOOLCHAIN_13_2_1=/opt/gcc-arm-none-eabi/bin' >> ~/.zshrc
source ~/.zshrc
```
