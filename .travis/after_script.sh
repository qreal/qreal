#!/bin/bash
set -euxo pipefail
case $TRAVIS_OS_NAME in
  osx)
    ;;
  linux)
    docker stop builder
    if [ "$TESTS" != "true" ]; then codecov --gcov-args '-o ${PWD}'; fi
    ;;
  *) exit 1 ;;
esac
