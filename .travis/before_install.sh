#!/bin/bash
set -euxo pipefail
case $TRAVIS_OS_NAME in
  osx)
#    brew update
    if [[ "$CONFIG" == "debug" ]]
    then
      python3.6 -m pip install --user codecov
    fi
    ;;
  linux)
    docker pull trikset/linux-builder
    if [[ "$CONFIG" == "debug" ]]
    then
      pip install --user --upgrade pip
      pip install --user codecov
    fi
    ;;
  *) exit 1 ;;
esac
