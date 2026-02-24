# USB CDC RX Sample App

## Description

This application is designed to measure serial write throughput. It sends a large amount of pre-generated data over a specified serial port and reports throughput in B/s, KB/s, or MB/s.

## Prerequisites
- Choose **one** setup path:
  - **CLI**: [Setup and Install SDK using CLI](../../../../docs/Astra_MCU_SDK_Setup_and_Install_CLI.md)
  - **VS Code**: [Setup and Install SDK using VS Code](../../../../docs/Astra_MCU_SDK_Setup_and_Install_VsCode.md)
- Install required Python package for `ser_test.py`:
  - `pyserial`

```bash
pip install pyserial
```

## Building and Flashing the Example using VS Code

Use the VS Code flow described in the SR110 guide and the VS Code Extension guide:
- [SR110 Build and Flash with VS Code](../../../../docs/SR110/SR110_Build_and_Flash_with_VSCode.md)
- [Astra MCU SDK VS Code Extension User Guide](../../../../docs/Astra_MCU_SDK_VSCode_Extension_User_Guide.md)

**Build (VS Code):**
1. Open **Build and Deploy** -> **Build Configurations**.
2. Select **usb_cdc_rx_sample_app** in the **Application** dropdown.
3. Build with **Build (SDK + App)** for the first build, or **Build App** for rebuilds.

**Flash (VS Code):**
1. Use **Image Conversion** to generate the flash image.
2. Use **Image Flashing** (SWD/JTAG) to flash the firmware image.

---

## Building and Flashing the Example using CLI

Use the CLI flow described in the SR110 guide:
- [SR110 Build and Flash with CLI](../../../../docs/SR110/SR110_Build_and_Flash_with_CLI.md)

**Build (CLI):**
1. From `<sdk-root>/examples`, build the example:
   ```bash
   cd <sdk-root>/examples
   export SRSDK_DIR=<sdk-root>
   make cm55_usb_cdc_rx_sample_app_defconfig BOARD=SR110_RDK BUILD=SRSDK
   ```

**Flash (CLI):**
1. Activate the SDK venv (required for image generation tools):
   ```bash
   # Linux/macOS
   source <sdk-root>/.venv/bin/activate
   # Windows PowerShell
   .\.venv\Scripts\Activate.ps1
   ```
2. Generate the flash image:
   ```bash
   cd <sdk-root>/tools/srsdk_image_generator
   python srsdk_image_generator.py \
     -B0 \
     -flash_image \
     -sdk_secured \
     -spk "<sdk-root>/tools/srsdk_image_generator/B0_Input_examples/spk_rc4_1_0_secure_otpk.bin" \
     -apbl "<sdk-root>/tools/srsdk_image_generator/B0_Input_examples/sr100_b0_bootloader_ver_0x012F_ASIC.axf" \
     -m55_image "<sdk-root>/examples/out/sr110_cm55_fw/release/sr110_cm55_fw.elf" \
     -flash_type "GD25LE128" \
     -flash_freq "67"
   ```
3. Flash the firmware image:
   ```bash
   cd <sdk-root>
   python tools/openocd/scripts/flash_xspi_tcl.py \
     --cfg_path tools/openocd/configs/sr110_m55.cfg \
     --image tools/srsdk_image_generator/Output/B0_Flash/B0_flash_full_image_GD25LE128_67Mhz_secured.bin \
     --erase-all
   ```

---

## Running the Application using VS Code Extension

1. Connect a USB cable to the application USB port on the SR110 board and press **RESET**.
2. For logging output, click **SERIAL MONITOR** and connect to the **DAP logger** port on J14.
   - To make it easier to identify, ensure **only J14** is plugged in (not J13).
   - The logger port is not guaranteed to be consistent across OSes. As a starting point:
     - **Windows:** try the lower-numbered J14 COM port first.
     - **Linux/macOS:** try the higher-numbered J14 port first.
   - If you do not see logs after a reset, switch to the other J14 port.
3. On **Windows**, convert **SYNA CDC 1** to a serial COM port before running `ser_test.py`:
   1. Open **Device Manager** and go to **Universal Serial Bus devices**.

      ![Universal Serial Bus Devices](assets/vs_syna_cdc_1.png)

   2. Right-click **SYNA CDC 1** and select **Update driver**.

      ![Update Driver](assets/vs_update_driver.png)

   3. Select **Browse my computer for drivers**.

      ![Browse my computer for drivers](assets/vs_browse.png)

   4. Select **Let me pick from a list of available drivers on my computer**.

      ![Let me pick from available driver from my computer](assets/vs_pick_available_drivers.png)

   5. Choose **USB Serial Device** and complete the installation.

      ![Select USB Serial Device](assets/vs_select_usb_serial_device.png)

   6. After the update, a new entry appears under **Ports (COM & LPT)** as **USB Serial Device (COMx)** (for example, `COM4`).

      ![New USB Port Entry](assets/vs_device_mng_result.png)

      - Use that `COMx` value as `<PORT_NAME>` in the script command.

4. Run the throughput test script on that USB CDC COM port:
   ```bash
   python ser_test.py <PORT_NAME>
   ```

**Expected Logs**

```
PS C:\Release_1.3.0\Astra_MCU_SDK1.3.0\examples\SR110_RDK\usb_examples\usb_cdc_rx_sample_app> python .\ser_test.py COM4
Testing port COM4
Throughput: 9.77 MB/s
```

## Features of Python Script

- **Serial Port Throughput Measurement:** Measures write throughput over serial.
- **Human-Readable Output:** Prints throughput in B/s, KB/s, or MB/s.
- **Simple Usage:** Requires only the serial port argument.
