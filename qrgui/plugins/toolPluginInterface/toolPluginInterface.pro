TEMPLATE = lib

DESTDIR = $$PWD/../../../bin

LIBS += -L$$PWD/../../../bin -lqrkernel

INCLUDEPATH = \
	$$PWD/ \
	$$PWD/../../ \
	$$PWD/../../../ \

HEADERS += \
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
