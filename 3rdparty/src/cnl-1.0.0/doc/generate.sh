#!/usr/bin/env bash

# Generate documentation in ./gh-pages

set -euo pipefail

PROJECT_DIR=$(
  cd "$(dirname "$0")"/..
  pwd
)

# Modify Doxyfile settings with environment variables.
export HTML_OUTPUT=${HTML_OUTPUT:-"${PROJECT_DIR}/doc/gh-pages"}
export CNL_VERSION=${CNL_VERSION:-"development"}
export CNL_DIR="${PROJECT_DIR}"

# Download std documentation.
wget -N http://upload.cppreference.com/mwiki/images/f/f8/cppreference-doxygen-web.tag.xml

# Generate CNL documentation.
doxygen "${PROJECT_DIR}/doc/Doxyfile"
