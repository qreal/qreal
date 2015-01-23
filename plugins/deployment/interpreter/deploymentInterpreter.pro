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
	$$PWD/shellWidget.h \
	$$PWD/blocks/shellBlock.h \
	$$PWD/blocks/incrementVersionInSourceCodeBlock.h \
	$$PWD/blocks/helpWithChangelogBlock.h \
	$$PWD/blocks/buildOnVirtualMachineBlock.h \
	$$PWD/blocks/uploadToGoogleDriveBlock.h \
	$$PWD/blocks/updateDownloadsCounterBlock.h \
	$$PWD/blocks/uploadToGoogleSiteBlock.h \

SOURCES += \
	$$PWD/deploymentInterpreterPlugin.cpp \
	$$PWD/blocksTable.cpp \
	$$PWD/shellWidget.cpp \
	$$PWD/blocks/shellBlock.cpp \
	$$PWD/blocks/incrementVersionInSourceCodeBlock.cpp \
	$$PWD/blocks/helpWithChangelogBlock.cpp \
	$$PWD/blocks/buildOnVirtualMachineBlock.cpp \
	$$PWD/blocks/uploadToGoogleDriveBlock.cpp \
	$$PWD/blocks/updateDownloadsCounterBlock.cpp \
	$$PWD/blocks/uploadToGoogleSiteBlock.cpp \

# qrgui includes
HEADERS +=  \
	$$PWD/../../../qrgui/mainWindow/projectManager/projectManagementInterface.h \
