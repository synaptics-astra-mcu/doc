# WiFi Driver Sample Application

## Description

The WiFi Driver sample application demonstrates SDIO-based WiFi bring-up on SR110 using the prebuilt WiFi libraries and shell tasks.

At startup, it initializes SDIO1 and then creates WiFi shell tasks used for WiFi test/control flows. Application status and diagnostics are printed through logger output.

## Hardware Requirements
- Astra Machina Micro Kit (SR110)
- **WLAN chip module 4612 or 43711 attached to the board** (required)
- **UART bridge adapter** for UART1 logging (required)

## Prerequisites
- Choose **one** setup path:
  - **CLI**: [Setup and Install SDK using CLI](../../../../docs/Astra_MCU_SDK_Setup_and_Install_CLI.md)
  - **VS Code**: [Setup and Install SDK using VS Code](../../../../docs/Astra_MCU_SDK_Setup_and_Install_VsCode.md)

## Building and Flashing the Example using VS Code

Use the VS Code flow described in the SR110 guide and the VS Code Extension guide:
- [SR110 Build and Flash with VS Code](../../../../docs/SR110/SR110_Build_and_Flash_with_VSCode.md)
- [Astra MCU SDK VS Code Extension User Guide](../../../../docs/Astra_MCU_SDK_VSCode_Extension_User_Guide.md)

**Build (VS Code):**
1. Open **Build and Deploy** -> **Build Configurations**.
2. Select **wifi_sample_app** in the **Application** dropdown.
3. Ensure logger interface is UART1 for this app.
   - Defconfig already enables UART1 console logging (`CONFIG_LOGGER_IF_UART_1_CONSOLE=y`).
   - If needed, update logger interface in Menuconfig before build.
4. Build with **Build (SDK + App)** for the first build, or **Build App** for rebuilds.

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
   make cm55_wifi_sample_app_defconfig BOARD=SR110_RDK BUILD=SRSDK
   ```
2. Ensure logger interface is UART1 (`CONFIG_LOGGER_IF_UART_1_CONSOLE=y`).
   If you need to edit config:
   ```bash
   make cm55_wifi_sample_app_defconfig BOARD=SR110_RDK BUILD=SRSDK EDIT=1
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

## Running the Application

1. Ensure the **4612/43711 WLAN chip is attached** to the board.
2. Connect UART bridge to **UART1** on Astra Machina Micro:
   - UART TX -> **GPIO 24**
   - UART RX -> **GPIO 23**
   - GND -> **Any GND pin on board**
3. Open a serial terminal on the UART bridge COM port (use the configured UART1 baud rate, default is 230400).
4. Press **RESET** on the board.
5. WiFi sample logs appear on the UART1 terminal.

**Expected Logs**

```
000000072.126:[WiFi] Joining AP

000000072.128:posix_rtos_init_semaphore, 806, 0x30011f78

000000072.231:mhd_wifi_join_specific, 1057, 5, 0000d099

000000072.243:mhd_wifi_join_specific, 1077, 0 0x0

000000074.183:event from idx 0, type 3, status 0, reason 0, flags 0

000000074.186:-- JOIN Event (type 3 status 0 reason 0)

000000074.189:event from idx 0, type 16, status 0, reason 0, flags 1

000000074.192:-- JOIN Event (type 16 status 0 reason 0)

000000074.225:event from idx 0, type 0, status 0, reason 0, flags 0

000000074.228:-- JOIN Event (type 0 status 0 reason 0)

000000074.231:event from idx 0, type 46, status 6, reason 0, flags 0

000000074.234:-- JOIN Event (type 46 status 262 reason 512)

000000074.237:[WiFi] Join successfully ! sta_is_link_up=1
 
```