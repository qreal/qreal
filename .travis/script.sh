#!/bin/bash
set -euxo pipefail
case $TRAVIS_OS_NAME in
  osx)
     export PATH="/usr/local/opt/qt/bin:$PATH"
     export PATH="/usr/local/opt/ccache/libexec:$PATH"
     EXECUTOR=
    ;;
  linux)
      docker run -d -v `pwd`:`pwd` -w `pwd` --name builder trikset/linux-builder Xvfb :0
      EXECUTOR="docker exec builder "
   ;;
  *) exit 1 ;;
esac

if [ "$VERA" = "true" ]; then $EXECUTOR .travis/runVera++.sh ; fi
$EXECUTOR qmake CONFIG+=$CONFIG CONFIG+=no-sanitizers QMAKE_CXX='ccache g++' -Wall $PROJECT.pro
$EXECUTOR make -k -j2 >/dev/null
$EXECUTOR sh -c "cd bin/$CONFIG && ls"
$EXECUTOR sh -c "export DISPLAY=:0 && cd bin/$CONFIG && $TESTS"
