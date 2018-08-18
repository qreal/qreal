#!/bin/bash
set -euxo pipefail
case $TRAVIS_OS_NAME in
  osx)
    brew install qt python@2 ccache
    ;;
  linux)
    ;;
  *) exit 1 ;;
esac
pkg-config --list-all
g++ --version

