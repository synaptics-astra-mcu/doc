# SPI Driver Sample Application

## Description

The SPI Driver sample application validates SPI data transfer on SR110 in multiple execution modes.

It supports:
- **Single-board mode:** controller and peripheral both run on the same board.
- **Double-board mode:** one board runs controller image, second board runs peripheral image.
- **Transfer styles:** blocking, non-blocking, and DMA-based transfer paths.

The app sends known data patterns and checks received data match counts on both controller and peripheral sides. Logs report pass/fail status and transfer timing.

## Hardware Requirements
- Astra Machina Micro Kit (SR110)
- UART bridge adapter (required for logging)
- SPI jumper wires
- For double-board mode: 2x Astra Machina Micro boards

## Prerequisites
- Choose **one** setup path:
  - **CLI**: [Setup and Install SDK using CLI](../../../../docs/Astra_MCU_SDK_Setup_and_Install_CLI.md)
  - **VS Code**: [Setup and Install SDK using VS Code](../../../../docs/Astra_MCU_SDK_Setup_and_Install_VsCode.md)

## Mode Configuration (Important)

Update `spi_sample_app.h` before build.

### Single-board non-blocking mode (requested baseline)
Set:
- `SPI_NON_BLOCKING_XFER = 1`
- `SPI_SINGLE_BOARD_MODE = 1`
- `SPI_DMA_XFER = 0`
- `SPI_DOUBLE_BOARD_MODE = 0`

### Single-board DMA mode
Set:
- `SPI_DMA_XFER = 1`
- Keep `SPI_SINGLE_BOARD_MODE = 1`
- Keep `SPI_DOUBLE_BOARD_MODE = 0`

### Double-board mode
Set:
- `SPI_DOUBLE_BOARD_MODE = 1`
- `SPI_SINGLE_BOARD_MODE = 0`
- Build two different images:
  - **Controller board image:** `SPI_CONTROLLER_MODE = 1`, `SPI_PERIPHERAL_MODE = 0`
  - **Peripheral board image:** `SPI_CONTROLLER_MODE = 0`, `SPI_PERIPHERAL_MODE = 1`

## Logger Interface Configuration

SPI sample logs are expected on **UART0 logger**.

Ensure logger interface is UART0 during build:
- `CONFIG_LOGGER_IF_UART_0=y`

**Important:** Enable `logger_if_uart0` when building SPI sample app.

(It is already enabled in `cm55_spi_sample_app_defconfig`, but verify if you changed config.)

## Wiring

### SPI wiring (single-board loopback)
Connect:
- `G24 -> G9`
- `G22 -> G6`
- `G23 -> G7`
- `G21 -> G8`

### SPI wiring (double-board mode)
Connect Controller board to Peripheral board:
- Controller `G22` -> Peripheral `G6`   (CLK)
- Controller `G21` -> Peripheral `G8`   (CS)
- Controller `G24` -> Peripheral `G7`   (MISO)
- Controller `G23` -> Peripheral `G9`   (MOSI)
- Connect GND between both boards

### UART0 logging wiring (UART bridge)
Connect UART bridge:
- `UART TX -> G45`
- `UART RX -> G44`
- `GND -> any board GND`

## Building and Flashing the Example using VS Code

Use the VS Code flow described in the SR110 guide and the VS Code Extension guide:
- [SR110 Build and Flash with VS Code](../../../../docs/SR110/SR110_Build_and_Flash_with_VSCode.md)
- [Astra MCU SDK VS Code Extension User Guide](../../../../docs/Astra_MCU_SDK_VSCode_Extension_User_Guide.md)

**Build (VS Code):**
1. Update `spi_sample_app.h` mode macros as required.
2. Ensure logger interface is UART0 (`CONFIG_LOGGER_IF_UART_0=y`).
3. Open **Build and Deploy** -> **Build Configurations**.
4. Select **spi_sample_app** in the **Application** dropdown.
5. Build with **Build (SDK + App)** for the first build, or **Build App** for rebuilds.

**Flash (VS Code):**
1. Use **Image Conversion** to generate the flash image.
2. Use **Image Flashing** (SWD/JTAG) to flash the firmware image.

---

## Building and Flashing the Example using CLI

Use the CLI flow described in the SR110 guide:
- [SR110 Build and Flash with CLI](../../../../docs/SR110/SR110_Build_and_Flash_with_CLI.md)

**Build (CLI):**
1. Update `spi_sample_app.h` mode macros as required.
2. Ensure logger interface is UART0 (`CONFIG_LOGGER_IF_UART_0=y`).
3. From `<sdk-root>/examples`, build the example:
   ```bash
   cd <sdk-root>/examples
   export SRSDK_DIR=<sdk-root>
   make cm55_spi_sample_app_defconfig BOARD=SR110_RDK BUILD=SRSDK
   ```
4. If you need to edit config (for logger/mode options):
   ```bash
   make cm55_spi_sample_app_defconfig BOARD=SR110_RDK BUILD=SRSDK EDIT=1
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

### Single-board mode
1. Complete SPI jumper wiring and UART0 bridge wiring listed above.
2. Flash SPI sample app image.
3. Press **RESET**.
4. Open serial terminal on UART bridge port and observe SPI transfer logs and match-count results.

### Double-board mode
1. Configure and build two images:
   - Controller-mode image
   - Peripheral-mode image
2. Flash controller image on board-1 and peripheral image on board-2.
3. Complete the double-board SPI wiring from the **Wiring** section.
4. Connect each board to a **different PC** (recommended) for independent flashing/logging.
5. Connect UART0 bridge for logs:
   - `UART TX -> G45`
   - `UART RX -> G44`
   - `GND -> any board GND`
6. Power both boards, then reset both boards (peripheral first, then controller if needed).
7. Open serial terminal(s) and monitor SPI transfer logs and match-count results.
