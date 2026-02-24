# USB CDC Echo Sample App

## Description

This application provides a simple command-line interface to connect to a serial port, send a message, and read the response from the connected device. It is useful for basic USB CDC communication testing.

## Prerequisites
- Choose **one** setup path:
  - **CLI**: [Setup and Install SDK using CLI](../../../../docs/Astra_MCU_SDK_Setup_and_Install_CLI.md)
  - **VS Code**: [Setup and Install SDK using VS Code](../../../../docs/Astra_MCU_SDK_Setup_and_Install_VsCode.md)
- Install required Python package for `serial_sender.py`:
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
2. Select **usb_cdc_echo_sample_app** in the **Application** dropdown.
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
   make cm55_usb_cdc_echo_sample_app_defconfig BOARD=SR110_RDK BUILD=SRSDK
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
3. On Windows, if required, update the COM port driver in Device Manager to **USB Serial Device**.
4. Run the sender script:
   ```bash
   python serial_sender.py -c <COM_PORT> -b <BAUD_RATE> -m "<MESSAGE_TO_SEND>"
   ```

**Expected Logs**

**Serial Monitor Logs**

```
---- Opened the serial port COM60 ----
0391470271:[0][WRN][LOGR]:Changing logger interface to LOGGER_IF_UART_1
0000000156:[0][INF][SYS ]:Application drivers initialization complete without errors.
0000004348:[0][INF][SYS ]:------------------------------------------
0000007710:[0][INF][SYS ]:            Hello  ASTRA                  
0000011076:[0][INF][SYS ]:------------------------------------------
0000014439:[0][INF][SYS ]:System initialization done
0000017035:[0][INF][SYS ]:sr110 SDK version 1.3.0
0000019526:[0][INF][USB ]:USB device unmounted
0000021858:[0][DBG][HAPI]:------------------------------------------
0000025220:[0][DBG][HAPI]:       Host API Router task               
0000028578:[0][DBG][HAPI]:------------------------------------------
0000031944:[0][INF][HAPI]:Active interface is USB
0000034404:[0][INF][USB ]:Usb CDC Echo Test Task Enter
0000037098:[0][INF][GENR]:Usecase manager service started.
0000039987:[0][DBG][HAPI]:A new service was registered, service ID: 6.
0000321147:[0][INF][USB ]:USB device mounted
```

**Terminal Logs**

```
PS C:\Release_1.3.0\Astra_MCU_SDK1.3.0\examples\SR110_RDK\usb_examples\usb_cdc_echo_sample_app> python .\serial_sender.py -c COM4 -b 230400 -m "Hello ASTRA!!"
Connected to COM4 at 230400 baud.
Sent: 'Hello ASTRA!!'
Response: HELLO ASTRA!!
```

## Features of Python Script

- **Customizable Serial Port:** Select COM port/device path.
- **Adjustable Baud Rate:** Configure baud rate as required.
- **Send Custom Messages:** Send custom payloads with CRLF.
- **Read Responses:** Read back response data from device.
- **Error Handling:** Prints useful errors for common serial issues.

