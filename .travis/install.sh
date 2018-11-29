#!/bin/bash
set -euxo pipefail
case $TRAVIS_OS_NAME in
  osx)
    REQUIRED_PACKAGES="qt ccache pyenv"
    export HOMEBREW_NO_AUTO_UPDATE=1
    for pkg in $REQUIRED_PACKAGES ; do
      p="${pkg##*/}"
      p="${p%.*}"
      rmdir $CELLAR_CACHE_DIR/$p && brew install $pkg \
      || { brew unlink $p ; brew link --force $p ; }
    done
    brew update && brew upgrade $REQUIRED_PACKAGES
    # export PYENV_ROOT="$CELLAR_CACHE_DIR/.pyenv"
    export PATH="$(pyenv root)/bin:$PATH"
    eval "$(pyenv init -)"
    #pyenv update || true
    pyenv install --list
    env CFLAGS="-O2 -fPIC" pyenv install -s 3.5.6
    ;;
  linux)
    docker pull trikset/linux-builder
    docker run -d -v $HOME:$HOME:rw -w `pwd` --name builder trikset/linux-builder Xvfb :0
    ;;
  *) exit 1 ;;
esac
