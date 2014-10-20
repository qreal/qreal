TEMPLATE = lib

DESTDIR = $$PWD/../../bin

TARGET = qrgui-mouse-gestures

LIBS += -L$$PWD/../../bin -lqrkernel -lqrutils

INCLUDEPATH = \
	$$PWD/ \
	$$PWD/../ \
	$$PWD/../../ \

QT += widgets

CONFIG += c++11

OBJECTS_DIR = .obj
UI_DIR = .ui
MOC_DIR = .moc
RCC_DIR = .moc

DEFINES += QRGUI_MOUSE_GESTURES_LIBRARY

TRANSLATIONS = $$PWD/../../qrtranslations/ru/qrgui_mouseGestures_ru.ts

HEADERS += \
	$$PWD/mouseGesturesDeclSpec.h \
	$$PWD/mouseMovementManager.h \
	$$PWD/gesturesWidget.h \
	$$PWD/gesturePainter.h \
	$$PWD/private/geometricForms.h \
	$$PWD/private/pathCorrector.h \
	$$PWD/private/levenshteinDistance.h \
	$$PWD/private/keyManager.h \
	$$PWD/private/keyBuilder.h \
	$$PWD/private/abstractRecognizer.h \
	$$PWD/private/mixedgesturesmanager.h \
	$$PWD/private/rectanglegesturesmanager.h \
	$$PWD/private/nearestposgridgesturesmanager.h \
	$$PWD/private/sorts.h \

SOURCES += \
	$$PWD/mouseMovementManager.cpp \
	$$PWD/gesturesWidget.cpp \
	$$PWD/gesturePainter.cpp \
	$$PWD/private/pathCorrector.cpp \
	$$PWD/private/levenshteinDistance.cpp \
	$$PWD/private/keyManager.cpp \
	$$PWD/private/keyBuilder.cpp \
	$$PWD/private/mixedgesturesmanager.cpp \
	$$PWD/private/rectanglegesturesmanager.cpp \
	$$PWD/private/nearestposgridgesturesmanager.cpp \

FORMS += \
	$$PWD/gesturesWidget.ui \
