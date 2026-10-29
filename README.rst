Synaptics SRSDK Documentation Sources
-------------------------------------

The compiled documentation is available at https://synaptics-astra-mcu.github.io/doc

To build the sources locally use the following command:::

  $ ./build.sh

Sidebar layout
--------------

The sidebar styles and script in ``_static/css/sidebar.css`` &
``_static/js/sidebar.js`` keep the navigation scrollbar visible and reserve
space for the fixed version selector added by the publishing workflow.
The scroll container fits within the sidebar width and its height excludes
the version selector, keeping the last navigation entry above the selector.
The reserved space follows the selector's height when it wraps or expands;
local builds without a version selector retain the theme's bottom spacing.
