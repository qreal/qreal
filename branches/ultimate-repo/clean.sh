#!/bin/sh

(cd ../source-generator && make distclean)
(cd reposerver && make distclean)
make distclean

# This is a workaround of qt4 bug #181577: QMAKE_[DIST]CLEAN does not remove
# directories
rm -rf .moc
rm -rf .obj
rm -rf reposerver/.moc
rm -rf reposerver/.obj
