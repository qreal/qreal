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
	$$PWD/src/kitPluginManager.h \
	$$PWD/src/robotsPlugin.h \
	$$PWD/src/robotModelManager.h \
	$$PWD/src/coreBlocks/coreBlocksFactory.h \
	$$PWD/src/coreBlocks/details/finalBlock.h \
	$$PWD/src/coreBlocks/details/initialBlock.h \
	$$PWD/src/interpreter/interpreter.h \
	$$PWD/src/interpreter/interpreterInterface.h \
	$$PWD/src/interpreter/details/autoconfigurer.h \
	$$PWD/src/interpreter/details/blocksTable.h \
	$$PWD/src/interpreter/details/robotsBlockParser.h \
	$$PWD/src/interpreter/details/thread.h \
	$$PWD/src/interpreter/details/sensorVariablesUpdater.h \
	$$PWD/src/ui/robotsSettingsPage.h \
	$$PWD/src/ui/sensorsConfigurationWidget.h \

SOURCES += \
	$$PWD/src/customizer.cpp \
	$$PWD/src/defaultRobotModel.cpp \
	$$PWD/src/kitPluginManager.cpp \
	$$PWD/src/robotsPlugin.cpp \
	$$PWD/src/robotModelManager.cpp \
	$$PWD/src/coreBlocks/coreBlocksFactory.cpp \
	$$PWD/src/coreBlocks/details/finalBlock.cpp \
	$$PWD/src/coreBlocks/details/initialBlock.cpp \
	$$PWD/src/interpreter/interpreter.cpp \
	$$PWD/src/interpreter/details/autoconfigurer.cpp \
	$$PWD/src/interpreter/details/blocksTable.cpp \
	$$PWD/src/interpreter/details/robotsBlockParser.cpp \
	$$PWD/src/interpreter/details/thread.cpp \
	$$PWD/src/interpreter/details/sensorVariablesUpdater.cpp \
	$$PWD/src/ui/robotsSettingsPage.cpp \
	$$PWD/src/ui/sensorsConfigurationWidget.cpp \

FORMS += \
	$$PWD/src/ui/robotsSettingsPage.ui \

TRANSLATIONS += $$PWD/interpreterCore_ru.ts

RESOURCES += \
	$$PWD/interpreterCore.qrc \

include($$PWD/qrguiIncludes.pri)
