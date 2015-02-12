TARGET = robots-trik-interpreter

include(../../../../global.pri)

TEMPLATE = lib
CONFIG += plugin

DESTDIR = $$DESTDIR/plugins/tools/kitPlugins/

include(src/trikKitInterpreter.pri)
