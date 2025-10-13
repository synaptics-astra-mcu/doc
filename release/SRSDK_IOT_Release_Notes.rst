Introduction 
============

   The Astra SR MCU from Synaptics is a cutting-edge, high-performance
   microcontroller designed to bring advanced AI and vision capabilities
   to a wide range of applications. The Astra SR MCU seamlessly
   integrates powerful processing, sophisticated vision systems, and
   exceptional energy efficiency into a compact package.

   Harness the power of artificial intelligence with the Astra SR MCU’s
   dual-core architecture and specialized neural network (NN)
   accelerators. Featuring an Arm® Cortex®-M55 CPU running up to 400MHz
   and an Ethos™-U55 micro NPU core, the Astra SR MCU™ delivers robust
   AI performance for real-time inferencing and tiered processing. The
   Astra SR MCU is equipped with the relevant interfaces to communicate
   with other devices in the system such as the application processor
   (AP), camera sensors, digital microphones, and other sensors.

   Astra SRSDK 1.1.0 Release is a software development kit supporting
   SR110 MCUs and which is a collection of source code files, tools and
   documents. SDK is based on CMSIS that can be compiled with GCC and
   AC6.

   The high-level components included in this SDK are described below:

-  Free RTOS

-  Source codes of various peripheral drivers, image processing, low
   power sensing blocks.

-  Applications showcasing camera-based sensing and wakeup, person
   detection/classification/pose detection, segmentation, JPEG pre-roll
   with frame transfer over USB CDC to host PC or SPI to another
   receiver (SPI capable device) and audio.

-  Astra SRSDK VSCode Extension now supports Docker-based build and
   image generation, along with integrated video streaming capabilities.

-  Related SDK Documents

General 
=======

====================== ==================================
   **Date**               22\ :sup:`nd` August 2025
====================== ==================================
   Version number         1.1
   Based on version       3.6.0
   Supported SoCs         SR110 Rev. B (B0) FCCSP & WLCSP
   Supported Platforms    SR110 RDK Board Rev A/B/C/D
   Release folder      
   File name              ASTRA_MCU_SDK1.1.0.tgz
   SYNA Toolkit           SynaToolkit v2.5
   APBL                   APBL v012F
   SPK                    SPK RC3.0
====================== ==================================

Supported Peripherals
=====================

==================== =====================
   ** HW Component**    **CM55-B0**
==================== =====================
   UART 0/1/LP          √
   IOMUX                √
   GPIO                 √
   DMA                  √ Memory to Memory
   I2C-S/M              √
   Cache                √
   Watchdog             √
   Clocks               √
   Timers               √
   LP-Sense             √
   USB 2.0 Device       √
   SPI-S/M           √
   I2S M                √
   xSPI (1/4 bits)      √
   MIPI CSI RX -0       √
   MIPI CSI RX -1       X
   DVP Serial           √
   DVP Parallel         X
   SDIO                 √
==================== =====================

.

Supported Software Components
=============================

================== ===========
**SW Component**   **CM55-B0**
================== ===========
Free RTOS          √
DMEM Service       √
Logger Service     √
USB Tiny USB Stack √
Flash Services     √
Host API Service   √
================== ===========

Supported Power Modes
=====================

+-----------------+---------------------------------+----------------+
|    ** Mode**    |    **Description**              |    **CM55-B0** |
+=================+=================================+================+
| **Active**      | Full Operational mode: Gear1    |    √           |
|                 | (G1), Gear2 (G2), Gear3 (G3)    |                |
|                 |                                 |    (400MHz)    |
|                 | Mode Entry: boot process        |                |
|                 |                                 |                |
|                 | Mode Exit: N/A                  |                |
+-----------------+---------------------------------+----------------+
| **Hibernation** |    All memories: in retention   |    √           |
|                 |                                 |                |
|                 |    Gear 3: (HP Proc) - disabled |                |
|                 |                                 |                |
|                 |    Gear 2: (LP Proc) - disabled |                |
|                 |                                 |                |
|                 |    Gear 1: (LP Sense) –         |                |
|                 |    disabled                     |                |
|                 |                                 |                |
|                 |    Gear 0 (AON): awake          |                |
|                 |                                 |                |
|                 |    Mode entry: Host API’s power |                |
|                 |    manager commands (0x1-0x4)   |                |
|                 |                                 |                |
|                 |    Mode exit: internal/external |                |
|                 |    event (timer, GPI)           |                |
+-----------------+---------------------------------+----------------+

VS Code Extension
=================

================ ===========
** SW Tool**     **CM55-B0**
================ ===========
Code Development √
Debug            √
Image Generator  √
Image Flashing   √
Video Streamer   √
================ ===========

SynaToolkit Suite
=================

==================================================== ===========
** SW Tool**                                         **CM55-B0**
==================================================== ===========
Logger                                               √
Control Interface (using embedded Host API protocol) √
Script Loader & Runner                               √
Video/Image Streamer                                 √
Image Generator                                      √
==================================================== ===========

Reference Applications
======================

============================= ========================
**Use Case**                  **CM55-B0 w/ Ethos U55**
============================= ========================
Vision: Person Detection      √
Vision: Person Classification √
Vision: Person Pose Detection √
Vision: Person Segmentation   √
Vision: Pre-roll              √
Audio: PDM to I2S             √
============================= ========================

Sample Applications
===================

=============================================== ========================
** Example**                                    **CM55-B0 w/ Ethos U55**
=============================================== ========================
   Inference - Single SRAM NN Model             √
Inference - Single Flash NN Model               √
UART Sample Application with flow control       √
I2C Sample Application                          √
PINMUX Sample Application                       √
GPIO Sample Application                         √
CLK Sample Application                          √
SPI Sample Application                          √
SDIO Sample Application (SDCARD & SDIO devices) √
FATFS Sample Application                        √
DMA Memory to Memory Sample Application         √
=============================================== ========================

.. _supported-sensors-and-configuration:

Supported Sensors and Configuration
===================================

+------------+-----------------+-----------------+-----------------+
| **Sensor** | **CSI           | **Parallel      | **Serial**      |
|            | configuration** | Configuration** |                 |
|            |                 |                 | **              |
|            |                 |                 | Configuration** |
+============+=================+=================+=================+
| OV2C       | -  SDR: 480x270 |                 |                 |
|            |    3FPS 160Mbps |                 |                 |
|            |    per lane,    |                 |                 |
|            |    lane count = |                 |                 |
|            |    1            |                 |                 |
|            |                 |                 |                 |
|            | -  SDR:         |                 |                 |
|            |    1920x1080    |                 |                 |
|            |    60FPS        |                 |                 |
|            |    1.5Gbps per  |                 |                 |
|            |    lane, lane   |                 |                 |
|            |    count = 1    |                 |                 |
+------------+-----------------+-----------------+-----------------+
| OV5647     | -  SDR: 480x270 |                 |                 |
|            |    30FPS        |                 |                 |
|            |    280Mbps per  |                 |                 |
|            |    lane, lane   |                 |                 |
|            |    count = 2    |                 |                 |
|            |                 |                 |                 |
|            | -  SDR: 640x480 |                 |                 |
|            |    60FPS        |                 |                 |
|            |    280Mbps per  |                 |                 |
|            |    lane, lane   |                 |                 |
|            |    count = 2    |                 |                 |
+------------+-----------------+-----------------+-----------------+
| K351       |    SDR: Serial  |                 |                 |
|            |    Camera       |                 |                 |
|            |    500x500 4FPS |                 |                 |
+------------+-----------------+-----------------+-----------------+

Supported Flash List
====================

========== =================
Vendor     Flash Part Number
========== =================
Macronix   MX25U12843G
GigaDevice GD25LE128E
WINBOND    W25Q128JW
========== =================

Known Issues
============

+------------+------------------------------+------------+----------+
| Issue      | Summary                      | Components | Severity |
+============+==============================+============+==========+
| SRSDKI-655 |    SPI Transfer gets         | SPI        | Major    |
|            |    corrupted beyond 1M       |            |          |
|            |    speed.                    |            |          |
+------------+------------------------------+------------+----------+
|            |                              |            |          |
+------------+------------------------------+------------+----------+

Revision History 
================

============== ======== ==============================
Last Modified  Revision    Description
============== ======== ==============================
   June 2025   0.1      Astra SRSDK Release 1.0.0
   August 2025    0.2      Astra SRSDK Release 1.1.0
\                       
\                       
\                       
============== ======== ==============================

Copyright

Copyright © 2024 Synaptics Incorporated. All Rights Reserved.

Trademarks

Synaptics; the Synaptics logo; are trademarks or registered trademarks
of Synaptics Incorporated in the United States and/or other countries.
All other trademarks are the property of their respective owners.

Notice

This document contains information that is proprietary to Synaptics
Incorporated (“Synaptics”). The holder of this document shall treat all
information contained herein as confidential, shall use the information
only for its intended purpose, and shall not duplicate, disclose, or
disseminate any of this information in any manner unless Synaptics has
otherwise provided express, written permission.

Use of the materials may require a license of intellectual property from
a third party or from Synaptics. This document conveys no express or
implied licenses to any intellectual property rights belonging to
Synaptics or any other party. Synaptics may, from time to time and at
its sole option, update the information contained in this document
without notice.

INFORMATION CONTAINED IN THIS DOCUMENT IS PROVIDED "AS-IS,” AND
SYNAPTICS HEREBY DISCLAIMS ALL EXPRESS OR IMPLIED

WARRANTIES, INCLUDING BUT NOT LIMITED TO ANY IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR

PURPOSE, AND ANY WARRANTIES OF NON-INFRINGEMENT OF ANY INTELLECTUAL
PROPERTY RIGHTS. IN NO EVENT SHALL SYNAPTICS BE

LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, PUNITIVE, OR
CONSEQUENTIAL DAMAGES ARISING OUT OF OR IN CONNECTION

WITH THE USE OF THE INFORMATION CONTAINED IN THIS DOCUMENT, HOWEVER
CAUSED AND BASED ON ANY THEORY OF LIABILITY,

WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
AND EVEN IF SYNAPTICS WAS ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. IF
A TRIBUNAL OF COMPETENT JURISDICTION DOES NOT PERMIT THE DISCLAIMER OF
DIRECT DAMAGES OR ANY OTHER DAMAGES, SYNAPTICS’ TOTAL CUMULATIVE
LIABILITY TO ANY PARTY SHALL NOT EXCEED ONE HUNDRED U.S. DOLLARS.

Ordering Information

For ordering information and a complete list of Synaptics' products,
contact your Synaptics sales representative. Visit our website at
www.synaptics.com to locate the Synaptics office nearest you.

Contact Us

Visit our website a\ `t www.synaptics.com
t <http://www.synaptics.com/>`__\ o locate the Synaptics office nearest
you.

   |image0|

.. |image0| image:: ./media/image4.jpg
   :width: 2.5in
   :height: 0.57292in
