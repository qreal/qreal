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

links(qrkernel qrrepo qrutils qrtext qrgui-models qrgui-controller qrgui-preferences-dialog qrgui-plugin-manager \
		qrgui-tool-plugin-interface robots-interpreter-base robots-utils robots-common-2d-model \
		)

HEADERS += \
	$$PWD/src/customizer.h \
	$$PWD/src/defaultRobotModel.h \
	$$PWD/src/robotsPlugin.h \
	$$PWD/src/robotsPluginFacade.h \
	$$PWD/src/coreBlocks/coreBlocksFactory.h \
	$$PWD/src/coreBlocks/details/timerBlock.h \
	$$PWD/src/coreBlocks/details/printTextBlock.h \
	$$PWD/src/coreBlocks/details/clearScreenBlock.h \
	$$PWD/src/interpreter/interpreter.h \
	$$PWD/src/interpreter/interpreterInterface.h \
	$$PWD/src/interpreter/details/autoconfigurer.h \
	$$PWD/src/interpreter/details/blocksTable.h \
	$$PWD/src/interpreter/details/sensorVariablesUpdater.h \
	$$PWD/src/managers/actionsManager.h \
	$$PWD/src/managers/blocksFactoryManager.h \
	$$PWD/src/managers/blocksFactoryManagerInterface.h \
	$$PWD/src/managers/graphicsWatcherManager.h \
	$$PWD/src/managers/kitPluginManager.h \
	$$PWD/src/managers/robotModelManager.h \
	$$PWD/src/managers/devicesConfigurationManager.h \
	$$PWD/src/managers/paletteUpdateManager.h \
	$$PWD/src/managers/saveConvertionManager.h \
	$$PWD/src/managers/kitAutoSwitcher.h \
	$$PWD/src/textLanguage/robotsBlockParser.h \
	$$PWD/src/ui/robotsSettingsPage.h \

SOURCES += \
	$$PWD/src/customizer.cpp \
	$$PWD/src/defaultRobotModel.cpp \
	$$PWD/src/robotsPlugin.cpp \
	$$PWD/src/robotsPluginFacade.cpp \
	$$PWD/src/coreBlocks/coreBlocksFactory.cpp \
	$$PWD/src/coreBlocks/details/timerBlock.cpp \
	$$PWD/src/coreBlocks/details/printTextBlock.cpp \
	$$PWD/src/coreBlocks/details/clearScreenBlock.cpp \
	$$PWD/src/interpreter/interpreter.cpp \
	$$PWD/src/interpreter/details/autoconfigurer.cpp \
	$$PWD/src/interpreter/details/blocksTable.cpp \
	$$PWD/src/interpreter/details/sensorVariablesUpdater.cpp \
	$$PWD/src/managers/actionsManager.cpp \
	$$PWD/src/managers/blocksFactoryManager.cpp \
	$$PWD/src/managers/graphicsWatcherManager.cpp \
	$$PWD/src/managers/kitPluginManager.cpp \
	$$PWD/src/managers/robotModelManager.cpp \
	$$PWD/src/managers/devicesConfigurationManager.cpp \
	$$PWD/src/managers/paletteUpdateManager.cpp \
	$$PWD/src/managers/saveConvertionManager.cpp \
	$$PWD/src/managers/kitAutoSwitcher.cpp \
	$$PWD/src/textLanguage/robotsBlockParser.cpp \
	$$PWD/src/ui/robotsSettingsPage.cpp \

FORMS += \
	$$PWD/src/ui/robotsSettingsPage.ui \

TRANSLATIONS += $$PWD/../../../../qrtranslations/ru/plugins/robots/interpreterCore_ru.ts

RESOURCES += \
	$$PWD/interpreterCore.qrc \

include($$PWD/qrguiIncludes.pri)
