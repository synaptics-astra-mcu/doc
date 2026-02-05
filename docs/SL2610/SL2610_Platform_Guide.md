# SL2610 Platform Guide

This document provides SL2610-specific hardware setup and platform notes. For build, image generation, and flashing workflows, use:

- [SL2610 Build and Flash with CLI](./SL2610_Build_and_Flash_with_CLI.md)
- [SL2610 Build and Flash with VS Code](./SL2610_Build_and_Flash_with_VSCode.md)

---

## Platform Overview

### SL2610 Specifications

**Processor:** Arm Cortex-M52 CPU

**Memory:** eMMC storage for boot images and filesystem

**Supported Development Kits:** **SL2610_RDK** (Astra Machina Micro SL Series)

---

## Hardware Setup

> **Important:** The official and up-to-date hardware documentation is maintained at: [Astra documentation portal](https://synaptics-astra.github.io/doc/v/latest/).
> That documentation takes precedence over the content in this section. This section is provided only as a convenience reference.

### SL2610 RDK

<figure>
<img src="../Assets/Images/media/SL2610_RDK.png" style="width:6.5in;height:auto" alt="SL2610 RDK Board" />
<figcaption><b>Figure 1.</b> SL2610 RDK Board</figcaption>
</figure>

### Connection Steps

#### 1. Power Connection

- Connect a **5V USB Type-C power adapter** to the **PWR IN** connector.
- Ensure the power adapter can supply sufficient current (recommended: 2A or higher).
- Power LED should illuminate when connected.

#### 2. Data Connection

- Connect the **USB 2.0 OTG port** to your host machine using a USB-C cable.
- This connection is used for:
  - eMMC flashing
  - System Manager image installation

#### 3. UART Logging Connection

For console output and debugging logs, connect a USB-to-UART adapter:

| Signal  | Board Pin | Description  | Notes                           |
|---------|-----------|--------------|---------------------------------|
| UART TX | Pin 8     | `SM_UART0_TX` | Board transmit → Host receive   |
| UART RX | Pin 28    | `SM_UART0_RX` | Board receive → Host transmit   |
| GND     | Pin 6     | Ground       | Common ground reference         |

**UART Settings:**
- **Baud rate**: 115200
- **Data bits**: 8
- **Parity**: None
- **Stop bits**: 1
- **Flow control**: None

**Connection diagram:**
```
Host USB-UART Adapter          SL2610 RDK
┌─────────────┐               ┌──────────┐
│     RX      │ ←───────────── │  Pin 8   │ (TX)
│     TX      │ ───────────→   │  Pin 28  │ (RX)
│     GND     │ ←────────────→ │  Pin 6   │ (GND)
└─────────────┘               └──────────┘
```

### Verify Hardware Setup

1. **Check power LED** is illuminated.
2. **Verify USB 2.0 OTG connection** shows up on host:
   - Windows: Device Manager
   - Linux: `lsusb` should show USB device
   - macOS: System Information → USB
3. **Test UART connection** using a serial terminal:
   - Open serial monitor (115200 baud, 8N1)
   - Reset board
   - Bootloader or system messages should appear


## References

- **VS Code workflows:** [Astra MCU SDK VS Code Extension User Guide](../Astra_MCU_SDK_VSCode_Extension_User_Guide.md)
- **CLI workflows:** [Astra MCU SDK User Guide](../Astra_MCU_SDK_User_Guide.md)

---

**Document Version:** 3.1
**Last Updated:** February 2026
**Supported Platforms:** SL2610_RDK (Astra Machina Micro SL Series)
