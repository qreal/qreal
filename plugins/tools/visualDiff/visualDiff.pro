TEMPLATE = lib
CONFIG += plugin c++11
QT += widgets
DESTDIR = ../../../bin/plugins/tools

MOC_DIR = .moc
RCC_DIR = .moc
UI_DIR = .ui
OBJECTS_DIR = .obj

LIBS += -L../../../bin -lqrkernel -lqslog -lqrutils -lqrrepo -lqrgui-tool-plugin-interface -lqrgui-models \
				-lqrgui-editor -lqrgui-controller -lqrgui-mouse-gestures -lqrgui-thirdparty -lqrgui-versioning-plugins-manager\

INCLUDEPATH = \
	$$PWD/ \
	$$PWD/../ \
	$$PWD/../../ \
	$$PWD/../../../ \
	$$PWD/../../../qrgui/ \

HEADERS += \
	$$PWD/diffPluginWrapper.h \
	$$PWD/model/diffModel.h \
	$$PWD/model/difference.h \
	$$PWD/model/diffState.h \
	$$PWD/model/modification.h \
	$$PWD/model/graphicalDifference.h \
	$$PWD/model/logicalDifference.h \
	$$PWD/model/commonDifference.h \
	$$PWD/model/elementType.h \
	$$PWD/model/modelLoader.h \
	$$PWD/view/diffColorPreferencesDialog.h \
	$$PWD/view/diffWindow.h \
	$$PWD/view/diffView.h \
	$$PWD/view/diffScene.h \
	$$PWD/view/detailsWidget/diffDetailsWidget.h \
	$$PWD/view/detailsWidget/idWidget.h \
	$$PWD/view/detailsWidget/parentWidget.h \
	$$PWD/view/detailsWidget/childrenWidget.h \
	$$PWD/view/detailsWidget/idListWidget.h \
	$$PWD/view/detailsWidget/propertiesWidget.h \
	$$PWD/view/detailsWidget/propertyWidget.h \
	$$PWD/view/detailsWidget/idListDiffWidget.h \

SOURCES += \
	$$PWD/diffPluginWrapper.cpp \
	$$PWD/model/diffModel.cpp \
	$$PWD/model/difference.cpp \
	$$PWD/model/modification.cpp \
	$$PWD/model/graphicalDifference.cpp \
	$$PWD/model/logicalDifference.cpp \
	$$PWD/model/commonDifference.cpp \
	$$PWD/model/modelLoader.cpp \
	$$PWD/view/diffColorPreferencesDialog.cpp \
	$$PWD/view/diffWindow.cpp \
	$$PWD/view/diffView.cpp \
	$$PWD/view/diffScene.cpp \
	$$PWD/view/detailsWidget/diffDetailsWidget.cpp \
	$$PWD/view/detailsWidget/idWidget.cpp \
	$$PWD/view/detailsWidget/parentWidget.cpp \
	$$PWD/view/detailsWidget/childrenWidget.cpp \
	$$PWD/view/detailsWidget/idListWidget.cpp \
	$$PWD/view/detailsWidget/propertiesWidget.cpp \
	$$PWD/view/detailsWidget/propertyWidget.cpp \
	$$PWD/view/detailsWidget/idListDiffWidget.cpp \

FORMS += \
	$$PWD/view/diffColorPreferencesDialog.ui
