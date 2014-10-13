QT += widgets

CONFIG += c++11

INCLUDEPATH += \
	$$PWD/../../.. \
	$$PWD/../../../qrgui/ \

LIBS += -L$$PWD/../../../bin -lqrkernel -lqrutils

HEADERS += \
    ../../../qrgui/dialogs/preferencesPages/preferencesPage.h
