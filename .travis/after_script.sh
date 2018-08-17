#!/bin/bash
set -euxo pipefail
case $TRAVIS_OS_NAME in
  osx)
    ;;
  linux)
    docker stop builder
    ;;
  *) exit 1 ;;
esac
if [ "$TESTS" != "true" ]; then codecov --gcov-exec ${GCOV} --gcov-args '-o ${PWD}'; fi

