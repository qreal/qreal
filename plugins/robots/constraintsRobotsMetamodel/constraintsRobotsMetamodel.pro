TEMPLATE = lib
CONFIG += plugin
DESTDIR = ../../../bin/plugins/
MOC_DIR = .moc
RCC_DIR = .moc

LIBS += -L../../../bin -lqrkernel -lqrutils -lqrrepo

HEADERS += \
	constraintsRobotsMetamodelPlugin.h \
	../../../qrgui/pluginManager/editorManagerInterface.h \
	constraintsRobotsDiagram_1.h \


SOURCES += \
	constraintsRobotsMetamodelPlugin.cpp \
	../../../qrgui/constraintsPluginInterface/checkStatus.cpp \
	constraintsRobotsDiagram_1.cpp \

