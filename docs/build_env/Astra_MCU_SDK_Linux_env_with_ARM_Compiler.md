# Setup Guide for Development Environment on Linux for Arm compiler
 
Content:

- [Setup Guide for Development Environment on Linux for Arm compiler](#setup-guide-for-development-environment-on-linux-for-arm-compiler)
  - [Install Basic Tools and Dependencies](#install-basic-tools-and-dependencies)
  - [Install CMake](#install-cmake)
  - [Install Ninja Build System](#install-ninja-build-system)
  - [Install Python](#install-python)
  - [Install the ARM Compiler](#install-the-arm-compiler)
  - [Debug Steps](#debug-steps)
   
### Install Basic Tools and Dependencies
First, update your package manager and install essential tools required for building and managing software projects:
 
```bash
sudo apt-get update -y && sudo apt-get -y install git wget make python3 zip unzip python-is-python3
```
 
### Install CMake
Download and install CMake version 4.1.2. CMake is vital for configuring, generating, and managing build processes in a platform-independent manner:
 
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

### Install Python
Python is required to run configuration tools (menuconfig, kconfig) and to execute scripts used during SDK build and setup.

```bash
sudo apt update
sudo apt install -y build-essential zlib1g-dev libncurses5-dev libgdbm-dev \
libnss3-dev libssl-dev libreadline-dev libffi-dev wget curl

curl https://pyenv.run | bash
export PATH="$HOME/.pyenv/bin:$PATH"
eval "$(pyenv init -)"
eval "$(pyenv virtualenv-init -)"

pyenv install 3.13.7
pyenv global 3.13.7
python3 --version
```

### Install the ARM Compiler
Download ARM Compiler 6.19 from [ARM_Compiler_for_linux](https://developer.arm.com/downloads)
 
```bash
sudo mkdir -p /home/temp
sudo tar -xzf ARMCompiler6.19_standalone_linux-x86_64.tar.gz -C /home/temp
cd /home/temp
sudo ./install_x86_64.sh --i-agree-to-the-contained-eula --no-interactive -d /home/arm_compiler/
export PATH=$PATH:/home/arm_compiler/bin
export ARMLMD_LICENSE_FILE=<license-file>
```

### Debug steps
For debug capabilities - download debian file of Ozone debugger from the link below:
https://www.segger.com/downloads/jlink/#Ozone
Then install it using the commands below:
```bash
sudo apt install build-essential
sudo dpkg --install Ozone_Linux_V334_x86_64.deb
```
