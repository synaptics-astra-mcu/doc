# Setup and Install SDK using VSCode

This guide provides a concise, end-to-end setup for the Astra MCU SDK in VS Code. For more detailed
information see [VS Code Extension User Guide](./Astra_MCU_SDK_VSCode_Extension_User_Guide.md)

## Prerequisites

- Supported host OS: Windows x64, Linux x86_64 or aarch64 (Ubuntu 22.04+), macOS x86_64 or ARM64.
- Visual Studio Code installed ([download](https://code.visualstudio.com/download)) and the `code` command available on your PATH.

Verify VS Code is available:

```bash
code --version
```

If `code` is not found, enable it using the platform steps below:

- macOS: In VS Code, open Command Palette and run `Shell Command: Install 'code' command in PATH`.
- Windows: Re-run the VS Code installer and select "Add to PATH", or add the VS Code install directory (for example `C:\Users\<username>\AppData\Local\Programs\Microsoft VS Code\bin`) to your PATH.
- Linux: Ensure the `code` binary is on PATH (for example `/usr/bin/code` from the package install), then restart your terminal.

Official references:
- macOS: https://code.visualstudio.com/docs/setup/mac#_launching-from-the-command-line
- Windows: https://code.visualstudio.com/docs/setup/windows#_command-line-usage
- Linux: https://code.visualstudio.com/docs/setup/linux#_launching-from-the-command-line

## Install the Synaptics VS Code Extension

1. Locate the VSIX package in `tools/` (for example, `Astra_MCU_SDK_vscode_extension-<version>.vsix`).
2. Install it from a terminal:

```bash
code --install-extension path/to/Astra_MCU_SDK_vscode_extension-<version>.vsix
```

3. Confirm the Synaptics extension appears in the VS Code activity bar.

    ![Synaptics VS Code Extension](./Assets/Images/media/vs_syna_logo.png)
4. Fully close and then reopen VS Code. 

## Install Tools

1. Open the Synaptics extension sidebar and select **Install Tools**.

    ![Install Tools Dialog](./Assets/Images/media/image_vs6.png)
2. The tool check runs automatically when the panel opens; you can also click "Check tool status" to re-run it.
3. Install any missing tools. Missing tools are flagged with a warning icon and are pre-selected for installation.

    ![Tools Check Status](./Assets/Images/media/image_vs9.png)
4. Use the default install location or select a custom directory:
   - Windows: `C:/Users/<username>/SRSDK_Build_tools`
   - Linux/macOS: `/home/<username>/SRSDK_Build_tools`
5. Click **Install** and monitor progress in the Install Script Terminal.

    ![Install Tools - Install Button](./Assets/Images/media/vs_1.3_install_tools.png)
4. During installation your OS my prompt for confirmation/approval on some installation steps
5. When installation finishes fully close VSCode and reopen. 

Notes:
- On Linux and macOS, the extension may prompt to install `jq` to update `settings.json`.
- Arm Compiler 6 (AC6) requires manual installation due to licensing constraints.

## Import Examples and Set SDK_DIR

1. In the Synaptics sidebar, choose **Import Application/Examples**.

    ![Synaptics Sidebar - Import Examples and Import SDK](./Assets/Images/media/image_vs100_k.png)
2. Import from local (select the SDK `examples/` folder) or remote (clone from repo).
3. After importing, open **Import SDK** and set `SRSDK_DIR` to the SDK root.

This updates the workspace `settings.json` so build, image conversion, and flashing can locate the SDK.

## Build and Flash (VS Code)

SoC-specific build and flash flows:
- SR110 quick guide: [SR110 Build and Flash with VS Code](./SR110/SR110_Build_and_Flash_with_VSCode.md)
- SR110: [VS Code Extension User Guide for SR110](./SR110/Astra_MCU_SDK_VSCode_Extension_User_Guide_SR110.md)
- SL2610: [VS Code Extension User Guide for SL2610](./SL2610/Astra_MCU_SDK_VSCode_Extension_User_Guide_SL2610.md)

## Troubleshooting Tips

- Only one SDK can be imported per VS Code workspace.
- On Windows, keep the SDK path length under 100 characters.
- If Build and Deploy is disabled, re-check `SRSDK_DIR` in **Import SDK**.
- Examples can only be built from the SDK `examples/` directory; import it via **Import Application/Examples**.
