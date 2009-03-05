#!/bin/sh

(cd source-generator && make distclean)
(cd source-generator && rm -rf generated)
(cd reposerver && make distclean)
make distclean

# This is a workaround of qt4 bug #181577: QMAKE_[DIST]CLEAN does not remove
# directories
rm -rf .moc
rm -rf .obj
rm -rf reposerver/.moc
rm -rf reposerver/.obj

# Remove generated files and dirs. Beware qt bug #181577!!!
rm -rf shapes
rm -rf real_dynamic.qrc
rm -rf umllib/uml_guiobjectfactory.cpp
rm -rf umllib/generated
rm -rf reposerver/generated
rm -rf repo/edges_stuff.cpp
rm -rf repo/realreporoles.h
rm -rf repo/realrepoinfo.cpp
rm -rf repo/realrepoinfo.h
