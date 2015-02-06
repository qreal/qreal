TARGET = robots-interpreter-core

include(../../../../global.pri)

TEMPLATE = lib
CONFIG += plugin

copyToDestdir($$PWD/icons/splashscreen.png)

include(interpreterCore.pri)
