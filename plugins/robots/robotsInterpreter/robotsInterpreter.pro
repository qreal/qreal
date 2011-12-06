QT += xml

TEMPLATE = lib
CONFIG += plugin
DESTDIR = ../../../bin/plugins/
MOC_DIR = .moc
RCC_DIR = .moc

LIBS += -L../../../bin -lqrkernel -lqrutils

CONFIG(debug, debug|release):LIBS  += -lqextserialportd
else:LIBS  += -lqextserialport

TRANSLATIONS = robotsInterpreter_ru.ts

HEADERS += \
	robotsPlugin.h \
	interpreter.h \
	sensorConstants.h \
	robotSettingsPage.h \
	customizer.h \
	watchListWindow.h \
	details/thread.h \
	details/blocksFactory.h \
	details/blocksTable.h \
	details/robotCommandConstants.h \
	details/robotsBlockParser.h \
	details/autoconfigurer.h \
	details/tracer.h \
	details/debugHelper.h \
    ui_watchListWindow.h \
    ../../../qrgui/dialogs/preferencesPages/preferencesPage.h

SOURCES += \
	robotsPlugin.cpp \
	interpreter.cpp \
	robotSettingsPage.cpp \
	watchListWindow.cpp\
	details/thread.cpp \
	details/blocksTable.cpp \
	details/blocksFactory.cpp \
	details/robotsBlockParser.cpp \
	details/autoconfigurer.cpp \
	details/tracer.cpp \
	details/debugHelper.cpp \

FORMS += \
	details/d2RobotModel/d2Form.ui \
	robotSettingsPage.ui \
    watchListWindow.ui

RESOURCES += \
	robotsInterpreter.qrc \

include(details/robotCommunication/robotCommunication.pri)

include(details/d2RobotModel/d2RobotModel.pri)

include(details/blocks/blocks.pri)

include(details/robotImplementations/robotImplementations.pri)

include(details/robotParts/robotParts.pri)
