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
	$$PWD/ \
	$$PWD/../interpreterCore/include/ \
	$$PWD/../interpreterBase/include/ \
	$$PWD/../commonTwoDModel/include/ \
	$$PWD/../../generators/generatorBase/include/ \
	$$PWD/../../utils/include/ \
	$$PWD/../../../../ \
	$$PWD/../../../../qrgui/ \
	$$PWD/../../../../qrtext/include \

LIBS += -L$$PWD/../../../../bin/ -lqslog -lqrkernel -lqrutils \
		-lqrgui-tool-plugin-interface -lqrgui-preferences-dialog -lqrgui-facade \
		-lqrgui-models -lqrgui-editor -lqrgui-plugin-manager -lqrgui-text-editor \
		-lrobots-utils -lrobots-interpreter-base -lrobots-interpreter-core -lrobots-common-2d-model \

TRANSLATIONS = $$PWD/../../../../qrtranslations/ru/plugins/robots/twoDModelRunner_ru.ts

!macx {
	QMAKE_LFLAGS="-Wl,-O1,-rpath,$$PWD/../../../../bin"
}

HEADERS += \
	$$PWD/runner.h \

SOURCES += \
	$$PWD/main.cpp \
	$$PWD/runner.cpp \
