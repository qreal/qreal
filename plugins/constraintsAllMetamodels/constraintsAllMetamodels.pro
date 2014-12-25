TEMPLATE = lib
CONFIG += plugin c++11
DESTDIR = ../../bin/plugins/tools
MOC_DIR = .moc
RCC_DIR = .moc

LIBS += -L../../bin -lqrkernel -lqrutils -lqrrepo

INCLUDEPATH += \
	$$PWD/../.. \
	$$PWD/../../qrgui/ \

HEADERS += \
	constraintsAllMetamodelsPlugin.h \
	../../qrgui/pluginManager/editorManagerInterface.h \
	constraintsAllLanguages.h \


SOURCES += \
	constraintsAllMetamodelsPlugin.cpp \
	../../qrgui/constraintsPluginInterface/checkStatus.cpp \
	constraintsAllLanguages.cpp \

