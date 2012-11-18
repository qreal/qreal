TEMPLATE = lib
CONFIG += plugin
DESTDIR = ../../../../bin/plugins/
MOC_DIR = .moc
RCC_DIR = .moc

LIBS += -L../../../../bin -lqrkernel -lqrutils -lqrrepo

HEADERS += \
	constraintsBlockDiagramMetamodelPlugin.h \
	../../../../qrgui/pluginManager/editorManagerInterface.h \
	constraintsBlockDiagram_1.h \
	constraintsBlockDiagram_2.h \


SOURCES += \
	constraintsBlockDiagramMetamodelPlugin.cpp \
	../../../../qrgui/constraintsPluginInterface/checkStatus.cpp \
	constraintsBlockDiagram_1.cpp \
	constraintsBlockDiagram_2.cpp \

