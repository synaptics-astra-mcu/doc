Synaptics ASTRA SRSDK Documentation
====================================

Welcome to the official documentation repository for the **Synaptics ASTRA SRSDK** (SR MCU Software Development Kit).

Overview
--------

The ASTRA SRSDK is a comprehensive software development kit designed for building advanced AI and vision applications on Synaptics Astra SR MCU platforms. This documentation provides everything you need to get started, develop, and deploy applications on the SR110 and other Astra MCU platforms.

📚 Documentation Sections
-------------------------

* **Getting Started** - Quick setup guides and initial configuration
* **User Guide** - Hands-on tutorials and demo applications
* **Developer Guide** - Comprehensive development resources, tools, and workflows
* **Subject-Specific Guides** - Detailed technical documentation for specialized topics
* **Platform Documentation** - Hardware specifications and platform-specific configurations
* **API Reference** - Complete peripheral driver library documentation
* **Release Notes** - Version history, updates, and known issues

🌐 Online Documentation
------------------------

The compiled documentation is available at:

* **Latest Release**: https://synaptics-astra-mcu.github.io/doc/

🔗 Related Resources
--------------------

* **Synaptics Corporate Website**: https://www.synaptics.com/
* **Astra Processor SDK**: https://synaptics-astra.github.io/doc/v/latest/

🛠️ Building Documentation Locally
-----------------------------------

Prerequisites
~~~~~~~~~~~~~

* Docker installed and running
* Access to the Synaptics Sphinx theme builder

Build Instructions
~~~~~~~~~~~~~~~~~~

**Using Docker (Recommended)**::

   sudo docker run --rm -v $(pwd):$(pwd) -w $(pwd) ghcr.io/syna-astra-dev/synaptics-sphinx-theme/builder:latest

The built HTML documentation will be available in the ``_build/html/`` directory.

📁 Repository Structure
-----------------------

::

  .
  ├── quickstart/          # Getting started guides
  ├── user_guide/          # User tutorials and examples
  ├── developer_guide/     # Developer documentation
  ├── subject/             # Subject-specific guides
  ├── platform/            # Platform documentation
  ├── release/             # Release notes
  ├── _static/             # Static assets (images, CSS, PDFs)
  ├── developer-guide/     # API reference (Doxygen generated)
  ├── conf.py              # Sphinx configuration
  ├── index.rst            # Main documentation index
  └── README.rst           # This file

🤝 Contributing
---------------

This documentation is maintained by the Synaptics ASTRA development team. For questions or issues, please contact the appropriate support channels.

📄 License
----------

Copyright © 2023-2025 Synaptics Incorporated. All rights reserved.
