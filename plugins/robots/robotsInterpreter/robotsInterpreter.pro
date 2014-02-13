QT += xml widgets network

TEMPLATE = lib
CONFIG += plugin
CONFIG += c++11

DESTDIR = ../../../bin/plugins/
OBJECTS_DIR = .obj
MOC_DIR = .moc
RCC_DIR = .moc

LIBS += -L../../../bin -lqrkernel -lqrutils -lqextserialport

INCLUDEPATH += \
	../../../ \
	../../../qrgui \

TRANSLATIONS = robotsInterpreter_ru.ts

HEADERS += \
	customizer.h \
	robotSettingsPage.h \
	robotsPlugin.h \
	sensorConstants.h \
	details/interpreter.h \
	details/thread.h \
	details/blocksFactory.h \
	details/blocksTable.h \
	details/robotCommandConstants.h \
	details/robotsBlockParser.h \
	details/autoconfigurer.h \
	details/tracer.h \
	details/debugHelper.h \
	details/timelineInterface.h \
	details/realTimeline.h \
	details/abstractTimer.h \
	details/realTimer.h \
	details/sensorsConfigurationManager.h \
	details/sensorsConfigurationProvider.h \
	details/sensorsConfigurationWidget.h \
	details/nullTimer.h \
	details/nxtDisplay.h \
	details/textExpressionProcessor.h \

SOURCES += \
	customizer.cpp \
	robotSettingsPage.cpp \
	robotsPlugin.cpp \
	sensorConstants.cpp \
	details/abstractTimer.cpp \
	details/autoconfigurer.cpp \
	details/blocksTable.cpp \
	details/blocksFactory.cpp \
	details/debugHelper.cpp \
	details/interpreter.cpp \
	details/nullTimer.cpp \
	details/nxtDisplay.cpp \
	details/realTimeline.cpp \
	details/realTimer.cpp \
	details/robotsBlockParser.cpp \
	details/sensorsConfigurationManager.cpp \
	details/sensorsConfigurationProvider.cpp \
	details/sensorsConfigurationWidget.cpp \
	details/thread.cpp \
	details/tracer.cpp \
	details/textExpressionProcessor.cpp \

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

include(qrguiIncludes.pri)
