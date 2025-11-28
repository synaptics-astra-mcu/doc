Doorbell application
********************

These applications are designed for doorbell systems, leveraging on-device machine learning to detect the presence of a person within the camera’s field of view. Upon detection, the system captures and processes images to provide visual context such as high-resolution stills or a sequence of preroll images leading up to the event. This use case combines features from multiple SDK sample applications, including jpeg_preroll and image_stitching, to deliver a comprehensive visual snapshot that enhances detection reliability and situational awareness.

.. toctree::
   :maxdepth: 1

   door_bell_readme
   serial_camera_door_bell_readme
   spi_preroll_protocol

:doc:`door_bell_readme`
   **MIPI Door Bell Application** - Complete application example demonstrating smart doorbell functionality with person detection and notification features.

:doc:`serial_camera_door_bell_readme`
   **Serial Camera Door Bell** - Advanced doorbell application using serial camera interface with enhanced features and connectivity options.

:doc:`spi_preroll_protocol`
   **SPI Preroll Protocol** - Technical guide for SPI-based data transfer protocols, including configuration and communication setup with external devices.
