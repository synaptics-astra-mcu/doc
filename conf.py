# Configuration file for the Sphinx documentation builder.
#
# For the full list of built-in configuration values, see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Project information -----------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#project-information

import sphinx_rtd_theme

# setup documentation metadata
project = 'Synaptics ASTRA SRSDK'
copyright = '2023 - 2025, Synaptics'
author = 'Synaptics'
release = 'SRSDK_v1.1.0'

# Custom project title for HTML
#html_title = f"{project} — Synaptics Astra SRSDK User Guide {release} documentation"

# -- General configuration ---------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#general-configuration

extensions = [
    "myst_parser",
    "synaptics_sphinx_theme",
]

#extensions = [ "synaptics_sphinx_theme", ]

exclude_patterns = [
        "README.rst",
        "org-docs/**"
        "_build",
        "Thumbs.db",
        ".DS_Store"
]

templates_path = ['_templates']

html_logo = "images/logo_full_white.png"


# -- Options for HTML output -------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#options-for-html-output

#html_theme = 'alabaster'



html_theme = 'sphinx_rtd_theme'
html_static_path = ['_static']

html_logo = "_static/images/logo_full_white.png"

html_theme_options = {
    'logo_only': False,
    'display_version': True,
    'prev_next_buttons_location': 'bottom',
    'style_external_links': False,
    'vcs_pageview_mode': '',
    'style_nav_header_background': '#007dc3',
    'collapse_navigation': False,
    'sticky_navigation': True,
    'navigation_depth': 4,
    'includehidden': True,
    'titles_only': False
}

html_context = {
  'display_github': False,
  'github_repo': 'syna-astra-mcu-dev.github.io',
  'github_version': 'main',
  'conf_py_path': '/',
  'version': release
}

# source_suffix configuration removed - let synaptics_sphinx_theme handle it
# source_suffix = {
#     ".rst": "restructuredtext",
#     ".md": "markdown",
# }

# Allow HTML files and directories to be included in the build output
html_extra_path = []

# Custom function to copy developer-guide with proper structure
import shutil
import os

def setup(app):
    def copy_developer_guide(app, exception):
        if exception is None:  # Only copy if build succeeded
            source_dir = os.path.join(app.srcdir, 'api_doc/developer-guide')
            target_dir = os.path.join(app.outdir, 'api_doc/developer-guide')
            if os.path.exists(source_dir):
                if os.path.exists(target_dir):
                    shutil.rmtree(target_dir)
                shutil.copytree(source_dir, target_dir)
                print(f"Copied {source_dir} to {target_dir}")
    
    app.connect('build-finished', copy_developer_guide)
    return {'version': '0.1'}

# MyST parser configuration removed - synaptics_sphinx_theme handles this
# myst_enable_extensions = [
#     "strikethrough",
#     "tasklist",
# ]
# myst_heading_anchors = 6

#html_theme_path = [sphinx_rtd_theme.get_html_theme_path()]
