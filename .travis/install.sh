#!/bin/bash
set -euxo pipefail
case $TRAVIS_OS_NAME in
  osx)
    REQURED_PACKAGES="qt ccache https://raw.githubusercontent.com/Homebrew/homebrew-core/f2a764ef944b1080be64bd88dca9a1d80130c558/Formula/python.rb"
    export HOMEBREW_NO_AUTO_UPDATE=1
    for pkg in $REQURED_PACKAGES ; do
      p="${pkg##*/}"
      p="${p%.*}"
      rmdir $CELLAR_CACHE_DIR/$p && brew install $pkg \
      || { brew unlink $p ; brew link --force $p ; }
    done
    ;;
  linux)
    docker run -d -v $HOME:$HOME:rw -w `pwd` --name builder trikset/linux-builder Xvfb :0
    ;;
  *) exit 1 ;;
esac
