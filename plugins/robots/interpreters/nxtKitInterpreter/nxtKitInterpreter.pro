TEMPLATE = lib
CONFIG += plugin
CONFIG += c++11

DESTDIR = ../../../../bin/plugins/tools/kitPlugins/
TARGET = robots-nxt-interpreter

OBJECTS_DIR = .obj
MOC_DIR = .moc
RCC_DIR = .moc
UI_DIR = .ui

include(src/nxtKitInterpreter.pri)
