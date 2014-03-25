QT += svg xml widgets

INCLUDEPATH += \
	$$PWD/src/ \
	$$PWD/../interpreterBase/include/ \
	$$PWD/../../utils/include/ \
	$$PWD/../../../../ \
	$$PWD/../../../../qrgui/ \

LIBS += -L$$PWD/../../../../bin/ -lqrkernel -lqrrepo -lqrutils -linterpreterBase -lutils

HEADERS += \
	$$PWD/src/customizer.h \
	$$PWD/src/defaultRobotModel.h \
	$$PWD/src/robotsPlugin.h \
	$$PWD/src/robotsPluginFacade.h \
	$$PWD/src/coreBlocks/coreBlocksFactory.h \
	$$PWD/src/coreBlocks/details/commentBlock.h \
	$$PWD/src/coreBlocks/details/engineCommandBlock.h \
	$$PWD/src/coreBlocks/details/enginesBackwardBlock.h \
	$$PWD/src/coreBlocks/details/enginesForwardBlock.h \
	$$PWD/src/coreBlocks/details/enginesStopBlock.h \
	$$PWD/src/coreBlocks/details/finalBlock.h \
	$$PWD/src/coreBlocks/details/initialBlock.h \
	$$PWD/src/coreBlocks/details/timerBlock.h \
	$$PWD/src/coreBlocks/details/waitBlock.h \
	$$PWD/src/coreBlocks/details/waitForSensorBlock.h \
	$$PWD/src/coreBlocks/details/waitForTouchSensorBlock.h \
	$$PWD/src/interpreter/interpreter.h \
	$$PWD/src/interpreter/interpreterInterface.h \
	$$PWD/src/interpreter/details/autoconfigurer.h \
	$$PWD/src/interpreter/details/blocksTable.h \
	$$PWD/src/interpreter/details/thread.h \
	$$PWD/src/interpreter/details/sensorVariablesUpdater.h \
	$$PWD/src/managers/actionsManager.h \
	$$PWD/src/managers/blocksFactoryManager.h \
	$$PWD/src/managers/blocksFactoryManagerInterface.h \
	$$PWD/src/managers/graphicsWatcherManager.h \
	$$PWD/src/managers/kitPluginManager.h \
	$$PWD/src/managers/robotModelManager.h \
	$$PWD/src/managers/sensorsConfigurationManager.h \
	$$PWD/src/managers/titlesVisibilityManager.h \
	$$PWD/src/textLanguage/robotsBlockParser.h \
	$$PWD/src/ui/robotsSettingsPage.h \
	$$PWD/src/ui/sensorsConfigurationWidget.h \

SOURCES += \
	$$PWD/src/customizer.cpp \
	$$PWD/src/defaultRobotModel.cpp \
	$$PWD/src/robotsPlugin.cpp \
	$$PWD/src/robotsPluginFacade.cpp \
	$$PWD/src/coreBlocks/coreBlocksFactory.cpp \
	$$PWD/src/coreBlocks/details/commentBlock.cpp \
	$$PWD/src/coreBlocks/details/engineCommandBlock.cpp \
	$$PWD/src/coreBlocks/details/enginesBackwardBlock.cpp \
	$$PWD/src/coreBlocks/details/enginesForwardBlock.cpp \
	$$PWD/src/coreBlocks/details/enginesStopBlock.cpp \
	$$PWD/src/coreBlocks/details/finalBlock.cpp \
	$$PWD/src/coreBlocks/details/initialBlock.cpp \
	$$PWD/src/coreBlocks/details/timerBlock.cpp \
	$$PWD/src/coreBlocks/details/waitBlock.cpp \
	$$PWD/src/coreBlocks/details/waitForSensorBlock.cpp \
	$$PWD/src/coreBlocks/details/waitForTouchSensorBlock.cpp \
	$$PWD/src/interpreter/interpreter.cpp \
	$$PWD/src/interpreter/details/autoconfigurer.cpp \
	$$PWD/src/interpreter/details/blocksTable.cpp \
	$$PWD/src/interpreter/details/thread.cpp \
	$$PWD/src/interpreter/details/sensorVariablesUpdater.cpp \
	$$PWD/src/managers/actionsManager.cpp \
	$$PWD/src/managers/blocksFactoryManager.cpp \
	$$PWD/src/managers/graphicsWatcherManager.cpp \
	$$PWD/src/managers/kitPluginManager.cpp \
	$$PWD/src/managers/robotModelManager.cpp \
	$$PWD/src/managers/sensorsConfigurationManager.cpp \
	$$PWD/src/managers/titlesVisibilityManager.cpp \
	$$PWD/src/textLanguage/robotsBlockParser.cpp \
	$$PWD/src/ui/robotsSettingsPage.cpp \
	$$PWD/src/ui/sensorsConfigurationWidget.cpp \

FORMS += \
	$$PWD/src/ui/robotsSettingsPage.ui \

TRANSLATIONS += $$PWD/interpreterCore_ru.ts

RESOURCES += \
	$$PWD/interpreterCore.qrc \

include($$PWD/qrguiIncludes.pri)
