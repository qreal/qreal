TEMPLATE = lib
CONFIG += plugin
DESTDIR = D:/My_EDUCATION/Qt4/YearlyProject/qreal/qreal/bin/plugins/
MOC_DIR = .moc
RCC_DIR = .moc

LIBS += -LD:/My_EDUCATION/Qt4/YearlyProject/qreal/qreal/bin -lqrkernel -lqrutils -lqrrepo

HEADERS += \
	constraintsRobotsMetamodelPlugin.h \
	D:/My_EDUCATION/Qt4/YearlyProject/qreal/qreal/qrgui/pluginManager/editorManagerInterface.h \
	constraintsRobotsDiagram_1.h \


SOURCES += \
	constraintsRobotsMetamodelPlugin.cpp \
	D:/My_EDUCATION/Qt4/YearlyProject/qreal/qreal/qrgui/constraintsPluginInterface/checkStatus.cpp \
	constraintsRobotsDiagram_1.cpp \

