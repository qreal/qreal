TEMPLATE = lib
CONFIG += plugin
DESTDIR = ../../bin/plugins/
MOC_DIR = .moc
RCC_DIR = .moc

LIBS += -L../../bin -lqrkernel -lqrutils -lqrrepo

HEADERS += \
	constraintsAllMetamodelsPlugin.h \
	../../qrgui/pluginManager/editorManagerInterface.h \
	constraintsAllLanguages_1.h \


SOURCES += \
	constraintsAllMetamodelsPlugin.cpp \
	../../qrgui/constraintsPluginInterface/checkStatus.cpp \
	constraintsAllLanguages_1.cpp \

