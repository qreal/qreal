@echo off
cd source-generator
make distclean
rmdir /q /s generated
cd ..
make distclean
cd reposerver 
make distclean
cd ..

rem This is a workaround of qt4 bug #181577: QMAKE_[DIST]CLEAN does not remove
rem directories
rmdir /q /s  .moc

rmdir /q /s .obj
rmdir /q /s reposerver\.moc
rmdir /q /s reposerver\.obj

rem Remove generated files and dirs. Beware qt bug #181577!!!
rmdir /q /s shapes
rmdir /q /s real_dynamic.qrc
rmdir /q /s umllib\uml_guiobjectfactory.cpp
rmdir /q /s umllib\generated
rmdir /q /s reposerver\generated
rmdir /q /s repo\edges_stuff.cpp
rmdir /q /s repo\realreporoles.h
rmdir /q /s repo\realrepoinfo.cpp
rmdir /q /s repo\realrepoinfo.h
