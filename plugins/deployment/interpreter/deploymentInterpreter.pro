QT += xml widgets

TEMPLATE = lib
CONFIG += plugin c++11

DESTDIR = ../../../bin/plugins/tools/
MOC_DIR = .moc
RCC_DIR = .moc

INCLUDEPATH += \
	$$PWD/../../.. \
	$$PWD/../../../qrgui/ \
	$$PWD/../../../qrtext/include/ \

LIBS += -L../../../bin -lqrkernel -lqrutils -lqrgui-preferences-dialog

TRANSLATIONS = $$PWD/../../../qrtranslations/ru/plugins/deploymentInterpreter_ru.ts

RESOURCES = deploymentInterpreter.qrc

HEADERS += \
	$$PWD/deploymentInterpreterPlugin.h \
	$$PWD/blocksTable.h \
	$$PWD/blocks/shellBlock.h \
	$$PWD/blocks/uploadToGoogleDriveBlock.h \
    shellWidget.h

SOURCES += \
	$$PWD/deploymentInterpreterPlugin.cpp \
	$$PWD/blocksTable.cpp \
	$$PWD/blocks/shellBlock.cpp \
	$$PWD/blocks/uploadToGoogleDriveBlock.cpp \
    shellWidget.cpp

# qrgui includes
HEADERS +=  \
	$$PWD/../../../qrgui/mainWindow/projectManager/projectManagementInterface.h \
