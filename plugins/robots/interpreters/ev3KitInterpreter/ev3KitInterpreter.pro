TARGET = robots-ev3-interpreter

include(../../../../global.pri)

TEMPLATE = lib
CONFIG += plugin

DESTDIR = $$DESTDIR/plugins/tools/kitPlugins/

include(src/ev3KitInterpreter.pri)
