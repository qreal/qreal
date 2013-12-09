TEMPLATE = lib

DESTDIR = ../bin

OBJECTS_DIR = .obj
MOC_DIR = .moc
RCC_DIR = .moc

TRANSLATIONS = $$PWD/qrutils_ru.ts

# Real-time plot
include(graphicsWatcher/sensorsGraph.pri)
include(qrutils.pri)
