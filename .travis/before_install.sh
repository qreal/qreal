#!/bin/bash
set -euxo pipefail
case $TRAVIS_OS_NAME in
  osx)
    brew update
    ;;
  linux)
    docker pull trikset/linux-builder
    if [[ "$CONFIG" == "debug" ]]
    then
      pip install --upgrade pip
      pip install --user codecov
    fi
    ;;
  *) exit 1 ;;
esac
