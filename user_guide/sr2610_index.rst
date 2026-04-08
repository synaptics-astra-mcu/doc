SR2610 User Guide
*****************

This section provides comprehensive user guides and demo applications for the SR2610 evaluation platform. Learn how to run Sample applications, configure peripheral, and explore the full capabilities of the SR2610 board through hands-on examples.

**Download Binaries (Path)**: All demo binaries can be downloaded from the GitHub repository under `GitHub Releases <https://github.com/synaptics-astra-mcu/srsdk_demos/tree/v1.2.0>`_

**Download Binaries (.zip)**: The pre-compiled demo binaries (ZIP package) are available at `GitHub Releases <https://github.com/synaptics-astra-mcu/srsdk_demos/releases/tag/v1.2.0>`_


.. toctree::
   :maxdepth: 1

   ../srsdk/examples/drivers_examples/dma_sample_app/README
   ../srsdk/examples/drivers_examples/i2c_exp_app/README
   ../srsdk/examples/drivers_examples/i2s_sample_app/README
   ../srsdk/examples/drivers_examples/spwm_sample_app/README
   ../srsdk/examples/drivers_examples/xspi_sample_app/README
   ../srsdk/examples/system_manager/README

**Sample Application Demos:**

:doc:`../srsdk/examples/drivers_examples/dma_sample_app/README`
   **DMA Sample App Demo** - Demonstrates advanced DMA usage including 1D memory transfers, linked descriptor chains, software-triggered transfers, channel management, pause/resume control, and interrupt-driven completion handling with data integrity validation.

:doc:`../srsdk/examples/drivers_examples/i2c_exp_app/README`
   **I2C Expander Sample App Demo** - Demonstrates GPIO output control using an external FXL6408 I2C-based GPIO expander with periodic toggling and status logging.

:doc:`../srsdk/examples/drivers_examples/i2s_sample_app/README`
   **I2S Sample App Demo** - Demonstrates I2S interface setup and audio data transfer flow for peripheral communication.

:doc:`../srsdk/examples/drivers_examples/spwm_sample_app/README`
   **SPWM Driver App Demo** - Configurable PWM framework for motor control and power electronics, supporting Normal PWM, PWM with Dead-Time, and Pseudo-Random PWM modes.

:doc:`../srsdk/examples/drivers_examples/xspi_sample_app/README`
   **XSPI Sample App Demo** - Demonstrates external SPI-NOR flash operations including initialization, JEDEC ID detection, erase, read/write operations, and throughput measurement.

:doc:`../srsdk/examples/system_manager/README`
   **System Manager Application** - Guide for SL2610 system manager boot flow handling, supported boot paths, and bring-up/runtime logging.


