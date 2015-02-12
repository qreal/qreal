TARGET = robots-null-interpreter

include(../../../../global.pri)

TEMPLATE = lib
CONFIG += plugin

DESTDIR = $$DESTDIR/plugins/tools/kitPlugins/

include(src/nullKitInterpreter.pri)
