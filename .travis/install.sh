#!/bin/bash
set -euxo pipefail
case $TRAVIS_OS_NAME in
  osx)
    brew install qt python@2 ccache
    ;;
  linux)
    docker run -d -v $HOME:$HOME:rw -w `pwd` --name builder trikset/linux-builder Xvfb :0
    ;;
  *) exit 1 ;;
esac
