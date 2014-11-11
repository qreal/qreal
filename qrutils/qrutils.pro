TEMPLATE = lib

DESTDIR = ../bin

OBJECTS_DIR = .obj
MOC_DIR = .moc
RCC_DIR = .moc

TRANSLATIONS = $$PWD/../qrtranslations/ru/qrutils_ru.ts

include(qrutils.pri)
