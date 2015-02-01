TARGET = robots-interpreter-core

include(../../../../global.pri)

TEMPLATE = lib
CONFIG += plugin

copyToDestdir(icons/splashscreen.png)

DESTDIR = $$DESTDIR/plugins/tools/

include(interpreterCore.pri)
