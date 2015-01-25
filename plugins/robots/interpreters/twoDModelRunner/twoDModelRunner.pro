TEMPLATE = app
CONFIG += c++11

DESTDIR = ../../../../bin/
TARGET = 2D-model

OBJECTS_DIR = .obj
MOC_DIR = .moc
RCC_DIR = .moc
UI_DIR = .ui

QT += widgets

INCLUDEPATH += \
	$$PWD/include/ \
	$$PWD/../interpreterBase/include/ \
	$$PWD/../../../../ \

LIBS += -L$$PWD/../../../../bin/ -lqslog -lqrkernel -lqrutils -lqrgui-tool-plugin-interface -lqrgui-preferences-dialog \
		-lrobots-utils -lrobots-interpreter-base -lrobots-common-2d-model \

TRANSLATIONS = $$PWD/../../../../qrtranslations/ru/plugins/robots/twoDModelRunner_ru.ts

HEADERS += \

SOURCES += \
	$$PWD/main.cpp \
