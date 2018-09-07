#!/bin/bash
set -euxo pipefail
case $TRAVIS_OS_NAME in
  osx)
     export PATH="/usr/local/opt/qt/bin:$PATH"
     export PATH="/usr/local/opt/ccache/libexec:$PATH"
     EXECUTOR=time
    ;;
  linux)
      docker run -d -v `pwd`:`pwd` -w `pwd` --name builder trikset/linux-builder Xvfb :0
      EXECUTOR="time docker exec builder "
   ;;
  *) exit 1 ;;
esac
export EXECUTOR
$EXECUTOR sh -c "export CCACHE_DIR=$HOME/.ccache/$TRAVIS_OS_NAME-$CONFIG"
$EXECUTOR which g++
if [ "$VERA" = "true" ]; then $EXECUTOR .travis/runVera++.sh ; fi
$EXECUTOR qmake -Wall CONFIG+=$CONFIG CONFIG+=no-sanitizers $PROJECT.pro
$EXECUTOR make -k -j2 qmake_all 1>>build.log 2>&1
$EXECUTOR make -k -j2 all 1>>build.log 2>&1
$EXECUTOR sh -c "cd bin/$CONFIG && ls"
$EXECUTOR sh -c "export DISPLAY=:0 && cd bin/$CONFIG && $TESTS"
