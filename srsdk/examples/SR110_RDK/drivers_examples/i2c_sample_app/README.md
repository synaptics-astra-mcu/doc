# I2C Driver Sample Application

## Description

The I2C Driver sample application demonstrates I2C controller/target communication on SR110 in both single-board and double-board setups.

What this sample does:
- Initializes I2C and applies controller/target configuration.
- Runs transfer flows in either blocking or non-blocking mode.
- Supports controller-transmit and target-transmit test directions.
- Verifies transferred bytes and prints match/mismatch results in logs.

In single-board mode, controller and target tasks run on the same board for internal loopback-style validation. In double-board mode, one board runs controller mode and the other board runs target mode.

## Hardware Requirements
- Astra Machina Micro Kit (SR110)

## Prerequisites
- Choose **one** setup path:
  - **CLI**: [Setup and Install SDK using CLI](../../../../docs/Astra_MCU_SDK_Setup_and_Install_CLI.md)
  - **VS Code**: [Setup and Install SDK using VS Code](../../../../docs/Astra_MCU_SDK_Setup_and_Install_VsCode.md)

## Mode Configuration (Important)

Update `i2c_sample_app.h` before build.

### 1. Board mode selection
Use only one board mode:
- `I2C_SINGLE_BOARD_MODE = 1`, `I2C_DOUBLE_BOARD_MODE = 0` (single-board)
- `I2C_SINGLE_BOARD_MODE = 0`, `I2C_DOUBLE_BOARD_MODE = 1` (double-board)

### 2. Double-board role selection
When `I2C_DOUBLE_BOARD_MODE = 1`, build separate images:
- **Controller board image:** `I2C_CONTROLLER_MODE = 1`, `I2C_TARGET_MODE = 0`
- **Target board image:** `I2C_CONTROLLER_MODE = 0`, `I2C_TARGET_MODE = 1`

### 3. Transfer type selection
Use one transfer type:
- `NON_BLOCKING_XFER = 1`, `BLOCKING_XFER = 0`
- or `NON_BLOCKING_XFER = 0`, `BLOCKING_XFER = 1`

### 4. Transfer direction selection
Use one direction:
- `CONTROLLER_TRANSMIT = 1`, `TARGET_TRANSMIT = 0`
- or `CONTROLLER_TRANSMIT = 0`, `TARGET_TRANSMIT = 1`

## Connections

### Single-board I2C loopback wiring
Connect I2C source pins to I2C target pins on the same board.

**RevB:**
- `GPIO_15 (I2C_SCL) -> GPIO_44 (I2C_SCL)`
- `GPIO_16 (I2C_SDA) -> GPIO_45 (I2C_SDA)`

**RevC and RevE:**
- `GPIO42 (I2C_SCL) -> GPIO_44 (I2C_SCL)`
- `GPIO43 (I2C_SDA) -> GPIO_45 (I2C_SDA)`

### Double-board wiring
Connect between two boards:
- Board-1 `SCL ->` Board-2 `SCL`
- Board-1 `SDA ->` Board-2 `SDA`
- Board-1 `GND ->` Board-2 `GND`

For double-board mode, ensure both GND pins are connected to maintain proper clock synchronization.

## Building and Flashing the Example using VS Code

Use the VS Code flow described in the SR110 guide and the VS Code Extension guide:
- [SR110 Build and Flash with VS Code](../../../../docs/SR110/SR110_Build_and_Flash_with_VSCode.md)
- [Astra MCU SDK VS Code Extension User Guide](../../../../docs/Astra_MCU_SDK_VSCode_Extension_User_Guide.md)

**Build (VS Code):**
1. Update `i2c_sample_app.h` macros for required mode.
2. Open **Build and Deploy** -> **Build Configurations**.
3. Select **i2c_sample_app** in the **Application** dropdown.
4. Build with **Build (SDK + App)** for the first build, or **Build App** for rebuilds.

**Flash (VS Code):**
1. Use **Image Conversion** to generate the flash image.
2. Use **Image Flashing** (SWD/JTAG) to flash the firmware image.

---

## Building and Flashing the Example using CLI

Use the CLI flow described in the SR110 guide:
- [SR110 Build and Flash with CLI](../../../../docs/SR110/SR110_Build_and_Flash_with_CLI.md)

**Build (CLI):**
1. Update `i2c_sample_app.h` macros for required mode.
2. From `<sdk-root>/examples`, build the example:
   ```bash
   cd <sdk-root>/examples
   export SRSDK_DIR=<sdk-root>
   make cm55_i2c_sample_app_defconfig BOARD=SR110_RDK BUILD=SRSDK
   ```
3. If you need to update config via menuconfig:
   ```bash
   make cm55_i2c_sample_app_defconfig BOARD=SR110_RDK BUILD=SRSDK EDIT=1
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

1. Complete the required I2C wiring from the **Connections** section.
2. Flash the image and press **RESET** on the board.
3. For logging output, click **SERIAL MONITOR** and connect to the **DAP logger** port on J14.
   - To make it easier to identify, ensure **only J14** is plugged in (not J13).
   - The logger port is not guaranteed to be consistent across OSes. As a starting point:
     - **Windows:** try the lower-numbered J14 COM port first.
     - **Linux/macOS:** try the higher-numbered J14 port first.
   - If you do not see logs after a reset, switch to the other J14 port.
4. Observe I2C configuration logs, transfer completion logs, and match/mismatch results.

### Double-board run note
- Build and flash controller-mode image to one board and target-mode image to the other board.
- Reset both boards after wiring is complete.

**Expected Logs**

**Controller Write**

```
0001666732:[0][WRN][LOGR]:Changing logger interface to LOGGER_IF_CONSOLE
0001781732:[0][INF][SYS ]:Application drivers
initialization complete without errors.
0001883107:[0][INF][SYS ]:SR110 SDK version 1.3.0
0001996986:[0][INF][GENR]:Starting I2C sample app...Target task started
0002101861:[0][INF][GENR]:Controller task started
0002204249:[0][DBG][GENR]:running task for loop 0x3000dfe0
0002305735:[0][INF][USB ]:USB Device Driver task
0002403497:[0][INF][USB ]:USB device unmounted
0002506487:[0][INF][USB ]:USB stopped
0003957107:[0][INF][GENR]:ten_bit_addr: 0,general_call_response:0,scl_stuck_timeout_ms:1,            sda_stuck_timeout_ms:1,target_address:50
0003957107:[0][INF][GENR]:0004121774:[0Target: Readi0004121855:[0][INF][GENR]:Controller target address: 0x32
0004222018:[0][INF][GENR]:Semaphore successfully taken
0004322121:[0][INF][GENR]:Controller: Sending data...
0005560586:[0][INF][GENR]:Write is completed
0005714488:[0][INF][GENR]:[Controller] I2C Event: 0x0
0005818112:[0][INF][GENR]:[Controller] I2C Event: 0x4
0005920614:[0][INF][GENR]:[Target] I2C Event: 0x1
0006029112:[0][INF][GENR]:[Target] I2C Event: 0x4
0006139512:[0][INF][GENR]:0006139512:[0][INF][GENR]:CTarget: Data received:RX[0] = 0x02
0006253498:[0][INF][GENR]:RX[1] = 0x01
0006356015:[0][INF][GENR]:0006356015:[00006356088:[0][INF][GENR]:RX[3] = 0x04
0006455873:[0][INF][GENR]:RX[4] = 0x05
0006568502:[0][INF][GENR]:RX[5] = 0x06
0006672998:[0][INF][GENR]:RX[6] = 0x07
0006778623:[0][INF][GENR]:RX[7] = 0x08
0006879748:[0][INF][GENR]:RX[8] = 0x29
0006985495:[0][INF][GENR]:RX[9] = 0x30
0007086498:[0][INF][GENR]:RX[10] = 0x31
0007185623:[0][INF][GENR]:RX[11] = 0x32
0007301771:[0][INF][HAPI]
```

**Target Write**

```
0001688861:[0][WRN][LOGR]:Changing logger interface to LOGGER_IF_CONSOLE
0001805737:[0][INF][SYS ]:Application drivers
initialization complete without errors
0001919612:[0][INF][SYS ]:SR110 SDK version 1.3.0
0002022116:[0][INF][GENR]:Starting I2C sample app...Target task started
0002129116:[0][INF][GENR]:Controller task started
0002238504:[0][DBG][GENR]:running task for loop 0x3000dfd8
0002353365:[0][INF][USB ]:USB Device Driver task
0002455627:[0][INF][USB ]:USB device unmounted
0002559117:[0][INF][USB ]:USB stopped
0005172865:[0][INF][GENR]:ten_bit_addr: 0,general_call_response:0,scl_stuck_timeout_ms:1,            sda_stuck_timeout_ms:1,target_address:50
0005172865:[0][INF][GENR]:ten_bit_addr: 0,enable_bus_clear:0,clear_stuck_sda:0,            scl_stuck_timeout_ms:1,sda_stuck_timeout_ms:1
0005430377:[0][INF][GENR]:Controller target address: 0x32
0006842213:[0][INF][GENR]:Semaphore successfully takenController: Reading starts...
0006921782:[0][INF][HAPI]:------------------------------------------
0007024131:[0][INF][HAPI]:       Host API Router task
0007125747:[0][INF][GENR]:[Controller] I2C Event: 0x1

0007338743:[0][INF][GENR]:[Target] I2C Event: 0x0
0007438867:[0][INF][GENR]:[Target] I2C Event: 0x4
0007541752:[0][INF][HAPI]:------------------------------------------
0007642253:[0][INF][HAPI]:Active interface is USB
0007753269:[0][INF][GENR]:0007753269:[0][INF][GENR]:CNon-Blocking read success with event type: 1
Non-Blocking read success with event type: 1
0Read byte [0]: 0x02
0007962151:[0][INF][GENR]:Non-Blocking target write with event type: 4
00008177285:[0][INF][GENR]:Read byte [2]: 0x03
0008275380:[0][INF][GENR]:Read byte [3]: 0x04
0008377005:[0][INF][GENR]:Read byte [4]: 0x05
0008477754:[0][INF][GENR]:Read byte [5]: 0x06
0008587879:[0][INF][GENR]:Read byte [6]: 0x07
0008693879:[0][INF][GENR]:Read byte [7]: 0x08
0008803505:[0][INF][GENR]:Read byte [8]: 0x29
0008905379:[0][INF][GENR]:Read byte [9]: 0x30
0009009379:[0][INF][GENR]:Read byte [10]: 0x31
0009122755:[0][INF][GENR]:Read byte [11]: 0x32
```
