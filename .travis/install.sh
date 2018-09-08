#!/bin/bash
set -euxo pipefail
case $TRAVIS_OS_NAME in
  osx)
    rmdir $CELLAR_CACHE_DIR/{qt,ccache} && brew install qt ccache \
    ||  { \
          brew link --force ccache ;  \
          brew link --force qt ; \
        }
    ;;
  linux)
    docker run -d -v $HOME:$HOME:rw -w `pwd` --name builder trikset/linux-builder Xvfb :0
    ;;
  *) exit 1 ;;
esac
