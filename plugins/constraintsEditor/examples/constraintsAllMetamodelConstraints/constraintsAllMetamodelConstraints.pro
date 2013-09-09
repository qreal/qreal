TEMPLATE = lib
CONFIG += plugin
DESTDIR = ../../qreal/bin/plugins/
MOC_DIR = .moc
RCC_DIR = .moc

LIBS += -L../../qreal/bin -lqrkernel -lqrutils -lqrrepo

HEADERS += \
	constraintsAllMetamodelsPlugin.h \
	../../qreal/qrgui/pluginManager/editorManagerInterface.h \
	constraintsAllLanguages_1.h \


SOURCES += \
	constraintsAllMetamodelsPlugin.cpp \
	../../qreal/qrgui/constraintsPluginInterface/checkStatus.cpp \
	constraintsAllLanguages_1.cpp \

