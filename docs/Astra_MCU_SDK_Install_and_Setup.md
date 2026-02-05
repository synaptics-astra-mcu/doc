# Setup guide for Install and Setup Python in Windows/Linux/Mac:

Content:
- [Setup guide for Python environment in windows, Linux, Mac](#setup-guide-for-install-and-setup-python-in-windowslinuxmac)
  - [Install Python and dependent package](#install-python-and-dependent-package)
    - [Windows Installation Steps](#windows-installation-steps)
    - [Linux Installation Steps](#linux-installation-steps)
    - [Mac Installation Steps](#mac-installation-steps)

## Install Python and dependent package

You can use the installation scripts provided with the VS Code extension to set up Python and all required dependencies for terminal-based usage. These scripts will:

 - Install Python (if it is not already available on your system)
 - Create a dedicated virtual environment
 - Install all necessary Python packages

This ensures a consistent and fully configured Python environment for running the SRSDK tools from the terminal.

### Windows Installation Steps

#### 1. Open PowerShell
Launch PowerShell with normal or administrator privileges.

#### 2. Run the Installation Script
Execute the following command (update the `<SRSDK_Dir>` and installation path accordingly):

```powershell
powershell.exe -ExecutionPolicy Bypass -File <SRSDK_Dir>\tools\vscode_extension\install_scripts\windows\install_tools.ps1 -InstallPath "<Path to Installation Directory>" python
```

**Notes:**
- Installs Python (if not already installed)
- Creates a virtual environment under:
  `C:\Users\<Username>\.sdk_venv\`
- Installs all required Python packages

#### 3. Activate the Virtual Environment

To activate the environment later, run:

```powershell
& 'C:\Users\<Username>\.sdk_venv\Scripts\Activate.ps1'
```

If you encounter the error `Activate.ps1 cannot be loaded because running scripts is disabled on this system`, run the below command first:

```powershell
powershell.exe -ExecutionPolicy Bypass
```

### Linux Installation Steps

#### 1. Open a Terminal
Launch your terminal with normal or sudo privileges.

#### 2. Run the Installation Script
Execute the following command (update the `<SRSDK_Dir>` and installation path accordingly):

```bash
sudo bash <SRSDK_Dir>/tools/vscode_extension/install_scripts/linux/install_tools.sh "<Path to Installation Directory>" python
```

**Notes:**
- Installs Python (if not already installed)
- Creates a virtual environment under:
  `/home/<username>/.sdk_venv/`
- Installs all required Python packages

#### 3. Activate the Virtual Environment

To activate the environment later, run:

```bash
source /home/<username>/.sdk_venv/bin/activate
```

### macOS Installation Steps

#### 1. Open a Terminal
Launch your terminal with normal or sudo privileges.

#### 2. Run the Installation Script
Execute the following command (update the `<SRSDK_Dir>` and installation path accordingly):

```bash
sudo bash <SRSDK_Dir>/tools/vscode_extension/install_scripts/mac/install_tools.sh "<Path to Installation Directory>" python
```

**Notes:**
- Installs Python (if not already installed)
- Creates a virtual environment under:
  `/Users/<username>/.sdk_venv/`
- Installs all required Python packages

#### 3. Activate the Virtual Environment

To activate the environment later, run:

```bash
source /Users/<username>/.sdk_venv/bin/activate
