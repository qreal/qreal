TEMPLATE = lib

DESTDIR = $$PWD/../../../bin

LIBS += -L$$PWD/../../../bin -lqrkernel

DEFINES += QRGUI_TOOL_PLUGIN_INTERFACE_LIBRARY

TRANSLATIONS = $$PWD/../../../qrtranslations/ru/qrgui_toolPluginInterface_ru.ts

INCLUDEPATH = \
	$$PWD/ \
	$$PWD/../../ \
	$$PWD/../../../ \

HEADERS += \
	$$PWD/toolPluginInterfaceDeclSpec.h \
	$$PWD/toolPluginInterface.h \
	$$PWD/customizer.h \
	$$PWD/pluginConfigurator.h \
	$$PWD/actionInfo.h \
	$$PWD/usedInterfaces/errorReporterInterface.h \
	$$PWD/usedInterfaces/details/modelsAssistInterface.h \
	$$PWD/usedInterfaces/graphicalModelAssistInterface.h \
	$$PWD/usedInterfaces/logicalModelAssistInterface.h \
	$$PWD/projectConverter.h \
	$$PWD/hotKeyActionInfo.h \
	$$PWD/systemEvents.h \
	$$PWD/pluginInterface.h \
