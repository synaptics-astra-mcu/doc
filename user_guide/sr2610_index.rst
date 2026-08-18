SL2610 User Guide
*****************

This section provides comprehensive user guides and demo applications for the SL2610 evaluation platform. Learn how to run Sample applications, configure peripheral, and explore the full capabilities of the SL2610 board through hands-on examples.

**Download Binaries (Path)**: All demo binaries can be downloaded from the GitHub repository under `GitHub Releases <https://github.com/synaptics-astra-mcu/srsdk_demos/tree/v1.2.0>`_

**Download Binaries (.zip)**: The pre-compiled demo binaries (ZIP package) are available at `GitHub Releases <https://github.com/synaptics-astra-mcu/srsdk_demos/releases/tag/v1.2.0>`_


.. toctree::
   :maxdepth: 1

   ../srsdk/examples/driver_examples/can_sample_app/README
   ../srsdk/examples/driver_examples/dma_sample_app/README
   ../srsdk/examples/driver_examples/dmic_sample_app/README
   ../srsdk/examples/driver_examples/eMMC_sample_app/README
   ../srsdk/examples/driver_examples/fc_sample_app/README
   ../srsdk/examples/driver_examples/i2c_exp_app/README
   ../srsdk/examples/driver_examples/i2s_sample_app/README
   ../srsdk/examples/driver_examples/i3c_sample_app/README
   ../srsdk/examples/driver_examples/nand_sample_app/README
   ../srsdk/examples/driver_examples/spwm_sample_app/README
   ../srsdk/examples/driver_examples/timer_sample_app/README
   ../srsdk/examples/driver_examples/watchdog_sample_app/README
   ../srsdk/examples/driver_examples/xspi_sample_app/README
   ../srsdk/examples/system_manager/README

**Sample Application Demos:**

:doc:`../srsdk/examples/driver_examples/can_sample_app/README`
   **CAN Sample App Demo** - A complete guide for the CAN Driver sample application, demonstrate Classical CAN and CAN FD communication with controller initialization, loopback testing, acceptance-filter validation, scheduled transmission, timestamp verification, and CAN FD payload/DLC validation.

:doc:`../srsdk/examples/driver_examples/dma_sample_app/README`
   **DMA Sample App Demo** - Demonstrates advanced DMA usage including 1D memory transfers, linked descriptor chains, software-triggered transfers, channel management, pause/resume control, and interrupt-driven completion handling with data integrity validation.

:doc:`../srsdk/examples/driver_examples/dmic_sample_app/README`
   **DMIC Sample App Demo** - A complete guide for the DMIC PDM-RX sample application, capture and verify digital microphone audio through FIFO and DMA-based receive paths, with support for blocking, non-blocking, and callback-driven capture modes, including PCM data validation and diagnostic logging.

:doc:`../srsdk/examples/driver_examples/eMMC_sample_app/README`
   **eMMC Sample App Demo** - Demonstrates eMMC bring-up, storage access, and basic read/write workflows for embedded flash storage validation.

:doc:`../srsdk/examples/driver_examples/fc_sample_app/README`
   **FC Sample App Demo** - A complete guide for the FC Driver sample application, demonstrate Fault Controller operation with controller initialization, fault-source routing, interrupt and reset validation, fault log capture, and callback completion verification.

:doc:`../srsdk/examples/driver_examples/i2c_exp_app/README`
   **I2C Expander Sample App Demo** - Demonstrates GPIO output control using an external FXL6408 I2C-based GPIO expander with periodic toggling and status logging.

:doc:`../srsdk/examples/driver_examples/i2s_sample_app/README`
   **I2S Sample App Demo** - Demonstrates I2S interface setup and audio data transfer flow for peripheral communication.

:doc:`../srsdk/examples/driver_examples/i3c_sample_app/README`
   **I3C Sample App Demo** - A complete guide for the I3C Driver sample application, demonstrate I3C controller and target operation with dynamic address assignment, CCC handling, blocking and non-blocking transfers, IBI/MR/Hot-Join events, I2C compatibility mode, and speed-test validation.

:doc:`../srsdk/examples/driver_examples/nand_sample_app/README`
   **Nand Sample App Demo** - Demonstrates initialization, JEDEC ID detection, erase, read, and write operations on an external SPI-NAND flash device.

:doc:`../srsdk/examples/driver_examples/spwm_sample_app/README`
   **SPWM Driver App Demo** - Configurable PWM framework for motor control and power electronics, supporting Normal PWM, PWM with Dead-Time, and Pseudo-Random PWM modes.

:doc:`../srsdk/examples/driver_examples/timer_sample_app/README`
   **TIMER Driver Sample App Demo** - A complete guide for the Timer Driver sample application, validate timer APIs, blocking delays, one-shot and periodic timer modes, system counter functions, remaining-time queries, timer cancellation, and optional slow-clock timer operation.

:doc:`../srsdk/examples/driver_examples/watchdog_sample_app/README`
   **Watchdog Driver Sample App Demo** - Demonstrates watchdog start, refresh, stop, restart, and timeout-reset validation through functional test scenarios on supported boards.

:doc:`../srsdk/examples/driver_examples/xspi_sample_app/README`
   **XSPI Sample App Demo** - Demonstrates external SPI-NOR flash operations including initialization, JEDEC ID detection, erase, read/write operations, and throughput measurement.

:doc:`../srsdk/examples/system_manager/README`
   **System Manager Application** - Guide for SL2610 system manager boot flow handling, supported boot paths, and bring-up/runtime logging.


