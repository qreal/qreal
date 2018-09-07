#!/bin/bash
set -euxo pipefail
$EXECUTOR ccache -sp
case $TRAVIS_OS_NAME in
  osx)
    ;;
  linux)
    docker stop builder
    if [ "$TESTS" != "true" ]; then codecov ; fi
    ;;
  *) exit 1 ;;
esac
