# Astra MCU SDK - WSL User Guide

This guide provides a concise setup flow for running the Astra MCU SDK on Windows using WSL2.

## Overview

WSL2 with Ubuntu 22.04 is the recommended environment for SL2610 development and is also a stable option for SR110.

## Requirements

- Windows 10/11 with WSL2 support enabled.
- Ubuntu 22.04 installed under WSL2.
- VS Code with the "Remote - WSL" extension installed.

## Setup Steps

1. Install and enable WSL2 on Windows:
   - [Install WSL](https://learn.microsoft.com/windows/wsl/install)
2. Install Ubuntu 22.04:
   - [Ubuntu 22.04 LTS on Microsoft Store](https://apps.microsoft.com/detail/9PN20MSR04DW)
3. Install and use VS Code with WSL:
   - [VS Code with WSL](https://code.visualstudio.com/docs/remote/wsl)
4. Update packages inside WSL:

   ```bash
   sudo apt update
   sudo apt install -y build-essential git python3 python3-venv cmake ninja-build
   ```

5. Open VS Code and connect to the WSL instance:
   - Install the "Remote - WSL" extension.
   - Use **Remote Explorer** or **WSL: New Window** to open the WSL environment.

6. Place the SDK in the WSL filesystem (recommended):
   - Use a path under `/home/<username>/` for better performance and file permissions.

7. Inside the WSL environment, continue with one of the setup guides:
   - [Setup and Install SDK using CLI](./Astra_MCU_SDK_Setup_and_Install_CLI.md)
   - [Setup and Install SDK using VS Code](./Astra_MCU_SDK_Setup_and_Install_VsCode.md)



## USB/Serial Device Access (WSL)

Flashing and serial access require USB device passthrough to WSL.

Use USBIPD on Windows:
- [USBIPD-WIN documentation](https://learn.microsoft.com/windows/wsl/connect-usb)

**Required flow (Windows PowerShell, Admin):**
1. List devices: `usbipd list`
2. Bind the target device in Admin mode with force:
   ```powershell
   usbipd bind --busid <BUSID> --force
   ```
3. In a separate (non-admin) terminal, attach to WSL:
   ```powershell
   usbipd attach --wsl --busid <BUSID> --auto-attach
   ```

**SL2610 note:** SL2610 re-enumerates three times. The first time you run the flashing tool you must bind each new enumeration (repeat the bind steps for each new enumeration). To do this you'll need to repeat the **USB_BOOT + RESET** sequence until all three enumerations have been bound. After that initial cycle, flashing will run normally.

**SR110 note:** Bind and attach both the Debug IC and the SR110 device. The SR110 will not appear in the USB list unless J13 is plugged in and the device has been reset.

**Unbind when done:** To use the USB device in Windows again, detach it from WSL and unbind it in an Admin PowerShell:
```powershell
usbipd detach --busid <BUSID>
usbipd unbind --busid <BUSID>
```

## Next Steps

- [Setup and Install SDK using CLI](./Astra_MCU_SDK_Setup_and_Install_CLI.md)
- [Setup and Install SDK using VS Code](./Astra_MCU_SDK_Setup_and_Install_VsCode.md)
