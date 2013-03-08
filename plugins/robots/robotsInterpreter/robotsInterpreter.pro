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
	../../../qrgui/dialogs/preferencesPages/preferencesPage.h \
	details/abstractTimer.h \
	details/realTimer.h \
	details/sensorsConfigurationWidget.h \
    details/nullTimer.h \
    graphicsWatcher/sensorGraph.h \
    graphicsWatcher/realTimePlot.h \
    graphicsWatcher/pointsQueueProcessor.h \
    graphicsWatcher/keyPoint.h

SOURCES += \
	robotsPlugin.cpp \
	interpreter.cpp \
	robotSettingsPage.cpp \
	customizer.cpp \
	watchListWindow.cpp\
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
    graphicsWatcher/sensorGraph.cpp \
    graphicsWatcher/realTimePlot.cpp \
    graphicsWatcher/pointsQueueProcessor.cpp \
    graphicsWatcher/keyPoint.cpp

FORMS += \
	details/d2RobotModel/d2Form.ui \
	robotSettingsPage.ui \
	watchListWindow.ui \
	details/sensorsConfigurationWidget.ui \
    graphicsWatcher/realTimePlot.ui

RESOURCES += \
	robotsInterpreter.qrc \

include(details/robotCommunication/robotCommunication.pri)

include(details/d2RobotModel/d2RobotModel.pri)

include(details/blocks/blocks.pri)

include(details/robotImplementations/robotImplementations.pri)

include(details/robotParts/robotParts.pri)

OTHER_FILES += \
    graphicsWatcher/zoomOut_btn.png \
    graphicsWatcher/zoomIn_btn.png \
    graphicsWatcher/stop_btn.png \
    graphicsWatcher/start_btn.png \
    graphicsWatcher/reset_btn.png
