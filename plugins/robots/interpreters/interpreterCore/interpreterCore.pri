QT += svg xml widgets

INCLUDEPATH += \
	$$PWD/src/ \
	$$PWD/../interpreterBase/include/ \
	$$PWD/../../generators/generatorBase/include/ \
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
	$$PWD/src/coreBlocks/details/enginesStopBlock.h \
	$$PWD/src/coreBlocks/details/finalBlock.h \
	$$PWD/src/coreBlocks/details/initialBlock.h \
	$$PWD/src/coreBlocks/details/ifBlock.h \
	$$PWD/src/coreBlocks/details/loopBlock.h \
	$$PWD/src/coreBlocks/details/forkBlock.h \
	$$PWD/src/coreBlocks/details/subprogramBlock.h \
	$$PWD/src/coreBlocks/details/functionBlock.h \
	$$PWD/src/coreBlocks/details/nullificationEncoderBlock.h \
	$$PWD/src/coreBlocks/details/timerBlock.h \
	$$PWD/src/coreBlocks/details/waitForSensorBlock.h \
	$$PWD/src/coreBlocks/details/waitForTouchSensorBlock.h \
	$$PWD/src/coreBlocks/details/waitForSonarDistanceBlock.h \
	$$PWD/src/coreBlocks/details/waitForColorSensorBlockBase.h \
	$$PWD/src/coreBlocks/details/waitForColorBlock.h \
	$$PWD/src/coreBlocks/details/waitForColorIntensityBlock.h \
	$$PWD/src/coreBlocks/details/waitForEncoderBlock.h \
	$$PWD/src/coreBlocks/details/waitForLightSensorBlock.h \
	$$PWD/src/coreBlocks/details/waitForSoundSensorBlock.h \
	$$PWD/src/coreBlocks/details/waitForGyroscopeBlock.h \
	$$PWD/src/coreBlocks/details/waitForAccelerometerBlock.h \
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
	$$PWD/src/managers/devicesConfigurationManager.h \
	$$PWD/src/managers/titlesVisibilityManager.h \
	$$PWD/src/managers/paletteUpdateManager.h \
	$$PWD/src/textLanguage/robotsBlockParser.h \
	$$PWD/src/ui/robotsSettingsPage.h \
	$$PWD/src/ui/devicesConfigurationWidget.h \

SOURCES += \
	$$PWD/src/customizer.cpp \
	$$PWD/src/defaultRobotModel.cpp \
	$$PWD/src/robotsPlugin.cpp \
	$$PWD/src/robotsPluginFacade.cpp \
	$$PWD/src/coreBlocks/coreBlocksFactory.cpp \
	$$PWD/src/coreBlocks/details/commentBlock.cpp \
	$$PWD/src/coreBlocks/details/enginesStopBlock.cpp \
	$$PWD/src/coreBlocks/details/finalBlock.cpp \
	$$PWD/src/coreBlocks/details/initialBlock.cpp \
	$$PWD/src/coreBlocks/details/ifBlock.cpp \
	$$PWD/src/coreBlocks/details/loopBlock.cpp \
	$$PWD/src/coreBlocks/details/forkBlock.cpp \
	$$PWD/src/coreBlocks/details/subprogramBlock.cpp \
	$$PWD/src/coreBlocks/details/functionBlock.cpp \
	$$PWD/src/coreBlocks/details/nullificationEncoderBlock.cpp \
	$$PWD/src/coreBlocks/details/timerBlock.cpp \
	$$PWD/src/coreBlocks/details/waitForSensorBlock.cpp \
	$$PWD/src/coreBlocks/details/waitForTouchSensorBlock.cpp \
	$$PWD/src/coreBlocks/details/waitForSonarDistanceBlock.cpp \
	$$PWD/src/coreBlocks/details/waitForColorSensorBlockBase.cpp \
	$$PWD/src/coreBlocks/details/waitForColorBlock.cpp \
	$$PWD/src/coreBlocks/details/waitForColorIntensityBlock.cpp \
	$$PWD/src/coreBlocks/details/waitForEncoderBlock.cpp \
	$$PWD/src/coreBlocks/details/waitForLightSensorBlock.cpp \
	$$PWD/src/coreBlocks/details/waitForSoundSensorBlock.cpp \
	$$PWD/src/coreBlocks/details/waitForGyroscopeBlock.cpp \
	$$PWD/src/coreBlocks/details/waitForAccelerometerBlock.cpp \
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
	$$PWD/src/managers/devicesConfigurationManager.cpp \
	$$PWD/src/managers/titlesVisibilityManager.cpp \
	$$PWD/src/managers/paletteUpdateManager.cpp \
	$$PWD/src/textLanguage/robotsBlockParser.cpp \
	$$PWD/src/ui/robotsSettingsPage.cpp \
	$$PWD/src/ui/devicesConfigurationWidget.cpp \

FORMS += \
	$$PWD/src/ui/robotsSettingsPage.ui \

TRANSLATIONS += $$PWD/interpreterCore_ru.ts

RESOURCES += \
	$$PWD/interpreterCore.qrc \

include($$PWD/qrguiIncludes.pri)
