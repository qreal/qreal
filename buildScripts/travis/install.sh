#!/bin/bash
set -euxo pipefail
case $TRAVIS_OS_NAME in
  osx)
    REQUIRED_PACKAGES="qt ccache pyenv coreutils"
    export HOMEBREW_TEMP="$HOME/homebrew.tmp"
    export HOMEBREW_LOGS="$HOMEBREW_TEMP"
    #To turn autoupdate on use `unset HOMEBREW_NO_AUTO_UPDATE` in a sub-shell before `breq install`
    export HOMEBREW_NO_AUTO_UPDATE=1
    for pkg in $REQUIRED_PACKAGES ; do
      p="${pkg##*/}"
      p="${p%.*}"
      rmdir $CELLAR_CACHE_DIR/$p && \
      ( brew install $pkg || echo "Failed to install $pkg" ) \
      || { brew unlink $p ; brew link --force $p ; }
    done
  ;;
  linux)
    docker pull trikset/linux-builder
    docker run -d -v $HOME:$HOME:rw -w `pwd` --name builder trikset/linux-builder Xvfb :0
    ;;
  *) exit 1 ;;
esac
