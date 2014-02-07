QT += xml widgets

INCLUDEPATH += \
	$$PWD/src/ \
	$$PWD/../interpreterBase/include/ \
	$$PWD/../../../../ \
	$$PWD/../../../../qrgui/ \

LIBS += -L$$PWD/../../../../bin/ -lqrkernel -lqrrepo -lqrutils -linterpreterBase

HEADERS += \
	$$PWD/src/customizer.h \
	$$PWD/src/defaultRobotModel.h \
	$$PWD/src/robotsPlugin.h \
	$$PWD/src/robotsPluginFacade.h \
	$$PWD/src/managers/actionsManager.h \
	$$PWD/src/managers/blocksFactoryManager.h \
	$$PWD/src/managers/blocksFactoryManagerInterface.h \
	$$PWD/src/managers/graphicsWatcherManager.h \
	$$PWD/src/managers/kitPluginManager.h \
	$$PWD/src/managers/robotModelManager.h \
	$$PWD/src/managers/sensorsConfigurationManager.h \
	$$PWD/src/managers/titlesVisibilityManager.h \
	$$PWD/src/coreBlocks/coreBlocksFactory.h \
	$$PWD/src/coreBlocks/details/finalBlock.h \
	$$PWD/src/coreBlocks/details/initialBlock.h \
	$$PWD/src/interpreter/interpreter.h \
	$$PWD/src/interpreter/interpreterInterface.h \
	$$PWD/src/interpreter/details/autoconfigurer.h \
	$$PWD/src/interpreter/details/blocksTable.h \
	$$PWD/src/interpreter/details/thread.h \
	$$PWD/src/interpreter/details/sensorVariablesUpdater.h \
	$$PWD/src/ui/robotsSettingsPage.h \
	$$PWD/src/ui/sensorsConfigurationWidget.h \
	$$PWD/src/textLanguage/robotsBlockParser.h \

SOURCES += \
	$$PWD/src/customizer.cpp \
	$$PWD/src/defaultRobotModel.cpp \
	$$PWD/src/robotsPlugin.cpp \
	$$PWD/src/robotsPluginFacade.cpp \
	$$PWD/src/managers/actionsManager.cpp \
	$$PWD/src/managers/blocksFactoryManager.cpp \
	$$PWD/src/managers/graphicsWatcherManager.cpp \
	$$PWD/src/managers/kitPluginManager.cpp \
	$$PWD/src/managers/robotModelManager.cpp \
	$$PWD/src/managers/sensorsConfigurationManager.cpp \
	$$PWD/src/managers/titlesVisibilityManager.cpp \
	$$PWD/src/coreBlocks/coreBlocksFactory.cpp \
	$$PWD/src/coreBlocks/details/finalBlock.cpp \
	$$PWD/src/coreBlocks/details/initialBlock.cpp \
	$$PWD/src/interpreter/interpreter.cpp \
	$$PWD/src/interpreter/details/autoconfigurer.cpp \
	$$PWD/src/interpreter/details/blocksTable.cpp \
	$$PWD/src/interpreter/details/thread.cpp \
	$$PWD/src/interpreter/details/sensorVariablesUpdater.cpp \
	$$PWD/src/ui/robotsSettingsPage.cpp \
	$$PWD/src/ui/sensorsConfigurationWidget.cpp \
	$$PWD/src/textLanguage/robotsBlockParser.cpp \

FORMS += \
	$$PWD/src/ui/robotsSettingsPage.ui \

TRANSLATIONS += $$PWD/interpreterCore_ru.ts

RESOURCES += \
	$$PWD/interpreterCore.qrc \

include($$PWD/qrguiIncludes.pri)
