QT += svg xml widgets

INCLUDEPATH += \
	$$PWD/include/ \
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
	$$PWD/include/interpreterCore/robotsPluginFacade.h \
	$$PWD/include/interpreterCore/managers/actionsManager.h \
	$$PWD/include/interpreterCore/managers/saveConvertionManager.h \
	$$PWD/include/interpreterCore/managers/blocksFactoryManager.h \
	$$PWD/include/interpreterCore/managers/blocksFactoryManagerInterface.h \
	$$PWD/include/interpreterCore/managers/graphicsWatcherManager.h \
	$$PWD/include/interpreterCore/managers/kitPluginManager.h \
	$$PWD/include/interpreterCore/managers/robotModelManager.h \
	$$PWD/include/interpreterCore/managers/devicesConfigurationManager.h \
	$$PWD/include/interpreterCore/managers/paletteUpdateManager.h \
	$$PWD/include/interpreterCore/managers/kitAutoSwitcher.h \
	$$PWD/include/interpreterCore/customizer.h \
	$$PWD/include/interpreterCore/defaultRobotModel.h \
	$$PWD/include/interpreterCore/interpreter/interpreter.h \
	$$PWD/include/interpreterCore/interpreter/interpreterInterface.h \
	$$PWD/include/interpreterCore/interpreter/details/autoconfigurer.h \
	$$PWD/include/interpreterCore/interpreter/details/blocksTable.h \
	$$PWD/include/interpreterCore/interpreter/details/sensorVariablesUpdater.h \
	$$PWD/include/interpreterCore/textLanguage/robotsBlockParser.h \
	$$PWD/src/coreBlocks/coreBlocksFactory.h \
	$$PWD/src/coreBlocks/details/timerBlock.h \
	$$PWD/src/coreBlocks/details/printTextBlock.h \
	$$PWD/src/coreBlocks/details/clearScreenBlock.h \
	$$PWD/src/managers/saveAsTaskManager.h \
	$$PWD/src/ui/robotsSettingsPage.h \
	$$PWD/src/ui/saveAsTaskDialog.h \

SOURCES += \
	$$PWD/src/customizer.cpp \
	$$PWD/src/defaultRobotModel.cpp \
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
	$$PWD/src/managers/devicesConfigurationManager.cpp \
	$$PWD/src/managers/graphicsWatcherManager.cpp \
	$$PWD/src/managers/kitAutoSwitcher.cpp \
	$$PWD/src/managers/kitPluginManager.cpp \
	$$PWD/src/managers/paletteUpdateManager.cpp \
	$$PWD/src/managers/robotModelManager.cpp \
	$$PWD/src/managers/saveAsTaskManager.cpp \
	$$PWD/src/managers/saveConvertionManager.cpp \
	$$PWD/src/textLanguage/robotsBlockParser.cpp \
	$$PWD/src/ui/robotsSettingsPage.cpp \
	$$PWD/src/ui/saveAsTaskDialog.cpp \

FORMS += \
	$$PWD/src/ui/robotsSettingsPage.ui \

TRANSLATIONS += $$PWD/../../../../qrtranslations/ru/plugins/robots/interpreterCore_ru.ts

RESOURCES += \
	$$PWD/interpreterCore.qrc \
