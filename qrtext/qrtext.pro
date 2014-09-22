TEMPLATE = lib

DESTDIR = ../bin

OBJECTS_DIR = .build/obj
MOC_DIR = .build/moc
RCC_DIR = .build/rcc

TRANSLATIONS = $$PWD/qrtext_ru.ts

include(qrtext.pri)
