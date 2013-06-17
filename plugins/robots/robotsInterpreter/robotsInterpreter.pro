QT += xml widgets

TEMPLATE = lib
CONFIG += plugin
DESTDIR = ../../../bin/plugins/
OBJECTS_DIR = .obj
MOC_DIR = .moc
RCC_DIR = .moc

LIBS += -L../../../bin -lqrkernel -lqrutils -lqextserialport

TRANSLATIONS = robotsInterpreter_ru.ts

HEADERS += \
robotsPlugin.h \
	interpreter.h \
	sensorConstants.h \
	robotSettingsPage.h \
	customizer.h \
	details/thread.h \
	details/blocksFactory.h \
	details/blocksTable.h \
	details/robotCommandConstants.h \
	details/robotsBlockParser.h \
	details/autoconfigurer.h \
	details/tracer.h \
	details/debugHelper.h \
	../../../qrgui/dialogs/preferencesPages/preferencesPage.h \
	details/abstractTimer.h \
	details/realTimer.h \
	details/sensorsConfigurationWidget.h \
	details/nullTimer.h \
	details/nxtDisplay.h \

SOURCES += \
	robotsPlugin.cpp \
	interpreter.cpp \
	robotSettingsPage.cpp \
	customizer.cpp \
	details/thread.cpp \
	details/blocksTable.cpp \
	details/blocksFactory.cpp \
	details/robotsBlockParser.cpp \
	details/autoconfigurer.cpp \
	details/tracer.cpp \
	details/debugHelper.cpp \
	details/abstractTimer.cpp \
	details/realTimer.cpp \
	details/sensorsConfigurationWidget.cpp \
	details/nullTimer.cpp \
	details/nxtDisplay.cpp \

FORMS += \
	details/d2RobotModel/d2Form.ui \
	robotSettingsPage.ui \
	details/sensorsConfigurationWidget.ui \
	details/nxtDisplay.ui \


RESOURCES += \
	robotsInterpreter.qrc \

include(details/robotCommunication/robotCommunication.pri)

include(details/d2RobotModel/d2RobotModel.pri)

include(details/blocks/blocks.pri)

include(details/robotImplementations/robotImplementations.pri)

include(details/robotParts/robotParts.pri)
