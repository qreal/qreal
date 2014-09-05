QT += svg xml widgets

INCLUDEPATH += \
	$$PWD/src/ \
	$$PWD/../interpreterBase/include/ \
	$$PWD/../commonTwoDModel/include/ \
	$$PWD/../../generators/generatorBase/include/ \
	$$PWD/../../utils/include/ \
	$$PWD/../../../../ \
	$$PWD/../../../../qrgui/ \
	$$PWD/../../../../qrtext/include \

LIBS += -L$$PWD/../../../../bin/ -lqrkernel -lqrrepo -lqrutils -linterpreterBase -lutils -lcommonTwoDModel -lqrtext

HEADERS += \
	$$PWD/src/customizer.h \
	$$PWD/src/defaultRobotModel.h \
	$$PWD/src/robotsPlugin.h \
	$$PWD/src/robotsPluginFacade.h \
	$$PWD/src/coreBlocks/coreBlocksFactory.h \
	$$PWD/src/coreBlocks/details/commentBlock.h \
	$$PWD/src/coreBlocks/details/finalBlock.h \
	$$PWD/src/coreBlocks/details/initialBlock.h \
	$$PWD/src/coreBlocks/details/ifBlock.h \
	$$PWD/src/coreBlocks/details/loopBlock.h \
	$$PWD/src/coreBlocks/details/forkBlock.h \
	$$PWD/src/coreBlocks/details/subprogramBlock.h \
	$$PWD/src/coreBlocks/details/functionBlock.h \
	$$PWD/src/coreBlocks/details/timerBlock.h \
	$$PWD/src/coreBlocks/details/variableInitBlock.h \
	$$PWD/src/coreBlocks/details/printTextBlock.h \
	$$PWD/src/coreBlocks/details/clearScreenBlock.h \
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
	$$PWD/src/managers/saveConvertionManager.h \
	$$PWD/src/managers/kitAutoSwitcher.h \
	$$PWD/src/managers/kitExtensionsUpdateManager.h \
	$$PWD/src/textLanguage/robotsBlockParser.h \
	$$PWD/src/textLanguage/newRobotsBlockParser.h \
	$$PWD/src/ui/robotsSettingsPage.h \

SOURCES += \
	$$PWD/src/customizer.cpp \
	$$PWD/src/defaultRobotModel.cpp \
	$$PWD/src/robotsPlugin.cpp \
	$$PWD/src/robotsPluginFacade.cpp \
	$$PWD/src/coreBlocks/coreBlocksFactory.cpp \
	$$PWD/src/coreBlocks/details/commentBlock.cpp \
	$$PWD/src/coreBlocks/details/finalBlock.cpp \
	$$PWD/src/coreBlocks/details/initialBlock.cpp \
	$$PWD/src/coreBlocks/details/ifBlock.cpp \
	$$PWD/src/coreBlocks/details/loopBlock.cpp \
	$$PWD/src/coreBlocks/details/forkBlock.cpp \
	$$PWD/src/coreBlocks/details/subprogramBlock.cpp \
	$$PWD/src/coreBlocks/details/functionBlock.cpp \
	$$PWD/src/coreBlocks/details/timerBlock.cpp \
	$$PWD/src/coreBlocks/details/variableInitBlock.cpp \
	$$PWD/src/coreBlocks/details/printTextBlock.cpp \
	$$PWD/src/coreBlocks/details/clearScreenBlock.cpp \
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
	$$PWD/src/managers/saveConvertionManager.cpp \
	$$PWD/src/managers/kitAutoSwitcher.cpp \
	$$PWD/src/textLanguage/robotsBlockParser.cpp \
	$$PWD/src/textLanguage/newRobotsBlockParser.cpp \
	$$PWD/src/ui/robotsSettingsPage.cpp \
	$$PWD/src/managers/kitExtensionsUpdateManager.cpp \

FORMS += \
	$$PWD/src/ui/robotsSettingsPage.ui \

TRANSLATIONS += $$PWD/../../../../qrtranslations/ru/plugins/robots/interpreterCore_ru.ts

RESOURCES += \
	$$PWD/interpreterCore.qrc \

include($$PWD/qrguiIncludes.pri)
