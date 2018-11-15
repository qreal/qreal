#!/bin/bash
set -euxo pipefail
case $TRAVIS_OS_NAME in
  osx)
     export PATH="/usr/local/opt/qt/bin:$PATH"
     export PATH="/usr/local/opt/ccache/libexec:$PATH"
     EXECUTOR=
     QMAKE_EXTRA="CONFIG+=no-sanitizers"
    ;;
  linux)
     EXECUTOR="time docker exec builder "
     QMAKE_EXTRA=
   ;;
  *) exit 1 ;;
esac
if $VERA ; then $EXECUTOR .travis/runVera++.sh ; fi
$EXECUTOR sh -c "export CCACHE_DIR=$HOME/.ccache/$TRAVIS_OS_NAME-$CONFIG \
&& export CCACHE_CPP2=yes \
&& export CCACHE_SLOPPINESS=time_macros \
&& which g++ \
&& g++ --version \
&& which qmake \
&& qmake -query \
&& ccache -M 0 \
&& pkg-config --list-all \
&& rm -f .qmake.cache \
&& qmake -Wall CONFIG+=$CONFIG $QMAKE_EXTRA $PROJECT.pro \
&& sh -c 'make -j2 qmake_all 1>>build.log 2>&1' \
&& sh -c 'make -j2 all 1>>build.log 2>&1' \
&& ccache -s \
&& sh -c \"cd bin/$CONFIG && ls\" \
&& sh -c \"export DISPLAY=:0 && cd bin/$CONFIG && $TESTS\""
