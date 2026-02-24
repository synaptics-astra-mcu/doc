# Pinmux Driver Sample Application

## Description

The Pinmux Driver sample application demonstrates how to configure SR110 pins for the required electrical and functional characteristics.

In simple terms, the sample shows how to tell a pin:
- **what role to perform** (for example, SPI/UART/GPIO function selection), and
- **how to configure electrical parameters** (drive strength, input enable, hold, pull-up/pull-down, slew rate, Schmitt trigger).

The app verifies key Pinmux APIs by:
- Setting and reading back full electrical configuration for a pin.
- Setting and reading back individual electrical parameters.
- Setting and reading back pin function mapping.
- Applying combined function + electrical config for one pin.
- Applying combined function + electrical config for multiple pins.

Each step prints status through logger so users can confirm configuration calls succeeded.

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
2. Select **pinmux_sample_app** in the **Application** dropdown.
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
   make cm55_pinmux_sample_app_defconfig BOARD=SR110_RDK BUILD=SRSDK
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

1. Press **RESET** on the board after flashing.
2. For logging output, click **SERIAL MONITOR** and connect to the **DAP logger** port on J14.
   - To make it easier to identify, ensure **only J14** is plugged in (not J13).
   - The logger port is not guaranteed to be consistent across OSes. As a starting point:
     - **Windows:** try the lower-numbered J14 COM port first.
     - **Linux/macOS:** try the higher-numbered J14 port first.
   - If you do not see logs after a reset, switch to the other J14 port.
3. Pinmux sample logs appear in the logger window, including pass/fail status for pin function and electrical configuration checks.

**Expected Logs**

```
0391469361:[0][WRN][LOGR]:Changing logger interface to LOGGER_IF_UART_1
0000000023:[0][INF][SYS ]:Application drivers initialization complete without errors.
0000004205:[0][INF][SYS ]:sr110 SDK version 1.3.0
0000006674:[0][INF][PMUX]:Pinmux set pin electrical characteristics test start
0000010516:[0][INF][PMUX]:Drive strength: 0, Enable_input: 1, Enable_hold :0, Pull_state:0, slew_rate:1, schmitt_trigger:0
0000016467:[0][INF][PMUX]:Pinmux set pin electrical characteristics test end
0000020203:[0][INF][PMUX]:Pinmux set pin electrical parameter test start
0000023759:[0][INF][PMUX]:Driver strength: 3
0000025980:[0][INF][PMUX]:input enable: 1
0000028053:[0][INF][PMUX]:Hold enable: 1
0000030082:[0][INF][PMUX]:pull state: 0
0000032059:[0][INF][PMUX]:slew rate: 0
0000033993:[0][INF][PMUX]:schmitt trigger: 1
0000036207:[0][INF][PMUX]:Pinmux set pin electrical parameter test end
0000039665:[0][INF][PMUX]:Pinmux set pin function test start
0000042647:[0][INF][PMUX]:Pin function: 1
0000044722:[0][INF][PMUX]:Pinmux set pin function test end
0000047608:[0][INF][PMUX]:Pinmux configuring both electrical characteristics and function for a pin is successful
0000053121:[0][INF][PMUX]:Pinmux configuring both electrical characteristics and function for multiple pins is successful
0000059020:[0][INF][PMUX]:All Pinmux Driver APIs functionalities are verified successfully!
```