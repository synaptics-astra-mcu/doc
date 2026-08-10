# Setup Guide for Development Environment on Linux for Arm Compiler

## Table of Contents
- [Install Basic Tools and Dependencies](#install-basic-tools-and-dependencies)
- [Install CMake](#install-cmake)
- [Install Ninja Build System](#install-ninja-build-system)
- [Install Python](#install-python)
- [Install OpenOCD](#install-openocd)
- [Install the ARM Compiler](#install-the-arm-compiler)
- [Debug Steps](#debug-steps)

## Install Basic Tools and Dependencies
First, update your package manager and install essential tools required for building and managing software projects:
 
```bash
sudo apt-get update -y && sudo apt-get -y install git wget make python3 zip unzip python-is-python3
```
 
## Install CMake
Download CMake 4.1.2 for Linux. CMake is vital for configuring, generating, and managing build processes in a platform-independent manner:
 
```bash
wget https://github.com/Kitware/CMake/releases/download/v4.1.2/cmake-4.1.2-linux-x86_64.sh
sudo bash ./cmake-4.1.2-linux-x86_64.sh --skip-license --exclude-subdir --prefix=/usr/local
```

## Install Ninja Build System
Ninja is a small build system with a focus on speed, which CMake can utilize to manage builds:

```bash
wget https://github.com/ninja-build/ninja/releases/download/v1.13.1/ninja-linux.zip
unzip ninja-linux.zip
mkdir /opt/ninja
mkdir /opt/ninja/bin
sudo cp ninja /opt/ninja/bin/
sudo chmod a+x /opt/ninja/bin/*
```
Add Ninja to PATH:
```bash
export PATH=/opt/ninja/bin:$PATH
```
Make it permanent:
```bash
echo 'export PATH=/opt/ninja/bin:$PATH' >> ~/.bashrc
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
sudo apt update
sudo apt install -y build-essential zlib1g-dev libncurses5-dev libgdbm-dev \
libnss3-dev libssl-dev libreadline-dev libffi-dev wget curl

curl https://pyenv.run | bash
export PATH="$HOME/.pyenv/bin:$PATH"
eval "$(pyenv init -)"
eval "$(pyenv virtualenv-init -)"

pyenv install 3.13.7
pyenv local 3.13.7
python3 --version
```

Optional: make pyenv available in future shells (skip this if you prefer to enable pyenv manually per shell):

```bash
echo 'export PATH="$HOME/.pyenv/bin:$PATH"' >> ~/.bashrc
echo 'eval "$(pyenv init -)"' >> ~/.bashrc
echo 'eval "$(pyenv virtualenv-init -)"' >> ~/.bashrc
```

If you chose the optional lines above, restart your shell or run `source ~/.bashrc` to apply them.

## Install OpenOCD

```bash
sudo apt-get update
sudo apt-get install -y openocd
openocd --version
```

## Install the ARM Compiler
Download Arm Compiler 6.19 for Linux: [**Arm Compiler 6.19**](https://developer.arm.com/downloads)
 
```bash
sudo mkdir -p /home/temp
sudo tar -xzf ARMCompiler6.19_standalone_linux-x86_64.tar.gz -C /home/temp
cd /home/temp
sudo ./install_x86_64.sh --i-agree-to-the-contained-eula --no-interactive -d /home/arm_compiler/
export PATH=$PATH:/home/arm_compiler/bin
export AC6_TOOLCHAIN_6_19_0=/home/arm_compiler/bin
export ARMLMD_LICENSE_FILE=<license-file>
```
Make it permanent:
```bash
echo 'export PATH=$PATH:/home/arm_compiler/bin' >> ~/.bashrc
echo 'export AC6_TOOLCHAIN_6_19_0=/home/arm_compiler/bin' >> ~/.bashrc
echo 'export ARMLMD_LICENSE_FILE=<license-file>' >> ~/.bashrc
```

## Debug Steps
For debug capabilities - download debian file of Ozone debugger from the link below:
[Segger Ozone download page](https://www.segger.com/downloads/jlink/#Ozone)
Then install it using the commands below:
```bash
sudo apt install build-essential
sudo dpkg --install Ozone_Linux_V334_x86_64.deb
```