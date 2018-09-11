#!/bin/bash
set -euxo pipefail
case $TRAVIS_OS_NAME in
  osx)
     export PATH="/usr/local/opt/qt/bin:$PATH"
     export PATH="/usr/local/opt/ccache/libexec:$PATH"
     EXECUTOR=
    ;;
  linux)
     EXECUTOR="time docker exec builder "
   ;;
  *) exit 1 ;;
esac
$EXECUTOR sh -c "export CCACHE_DIR=$HOME/.ccache/$TRAVIS_OS_NAME-$CONFIG \
&& which g++ \
&& g++ --version \
&& ccache -M 0 \
&& pkg-config --list-all \
&& if $VERA ; then .travis/runVera++.sh ; fi \
&& qmake -Wall CONFIG+=$CONFIG CONFIG+=no-sanitizers QMAKE_CXX=\"ccache $CXX\" $PROJECT.pro \
&& sh -c 'make -k -j2 qmake_all # 1>>build.log 2>&1' \
&& sh -c 'make -k -j2 all #1>>build.log 2>&1' \
&& ccache -s \
&& sh -c \"cd bin/$CONFIG && ls\" \
&& sh -c \"export DISPLAY=:0 && cd bin/$CONFIG && $TESTS\""
