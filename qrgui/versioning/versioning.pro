TEMPLATE = lib

DESTDIR = $$PWD/../../bin

TARGET = qrgui-versioning-plugins-manager

QT += widgets

LIBS += -L$$PWD/../../bin -lqrkernel -lqslog -lqrutils -lqrrepo -lqrgui-tool-plugin-interface -lqrgui-models \
			    -lqrgui-editor -lqrgui-controller -lqrgui-mouse-gestures -lqrgui-thirdparty \

DEFINES += QRGUI_VERSIONING_PLUGINS_MANAGER_LIBRARY

INCLUDEPATH = \
	$$PWD/ \
	$$PWD/../../ \
	$$PWD/../../../ \

CONFIG += c++11

OBJECTS_DIR = .obj
UI_DIR = .ui
MOC_DIR = .moc
RCC_DIR = .moc

TRANSLATIONS = $$PWD/../../../qrtranslations/ru/qrgui_versioningPluginsManager_ru.ts

HEADERS += \
	$$PWD/versioningPluginsManagerDeclSpec.h \
	$$PWD/versioningPluginsManager.h \
	$$PWD/versioningPluginInterface.h \
	$$PWD/diffPluginBase.h \
	$$PWD/diffPluginInterface.h \

SOURCES += \
	$$PWD/versioningPluginsManager.cpp \
	$$PWD/diffPluginBase.cpp \

# Visual diff
# This a temp solution till system would be refactored
#  the components could be reused in plugins
HEADERS += \
	$$PWD/visualDiff/diffPluginWrapper.h \
	$$PWD/visualDiff/model/diffModel.h \
	$$PWD/visualDiff/model/difference.h \
	$$PWD/visualDiff/model/diffState.h \
	$$PWD/visualDiff/model/modification.h \
	$$PWD/visualDiff/model/graphicalDifference.h \
	$$PWD/visualDiff/model/logicalDifference.h \
	$$PWD/visualDiff/model/commonDifference.h \
	$$PWD/visualDiff/model/elementType.h \
	$$PWD/visualDiff/model/modelLoader.h \
	$$PWD/visualDiff/view/diffColorPreferencesDialog.h \
	$$PWD/visualDiff/view/diffWindow.h \
	$$PWD/visualDiff/view/diffView.h \
	$$PWD/visualDiff/view/diffScene.h \
	$$PWD/visualDiff/view/detailsWidget/diffDetailsWidget.h \
	$$PWD/visualDiff/view/detailsWidget/idWidget.h \
	$$PWD/visualDiff/view/detailsWidget/parentWidget.h \
	$$PWD/visualDiff/view/detailsWidget/childrenWidget.h \
	$$PWD/visualDiff/view/detailsWidget/idListWidget.h \
	$$PWD/visualDiff/view/detailsWidget/propertiesWidget.h \
	$$PWD/visualDiff/view/detailsWidget/propertyWidget.h \
	$$PWD/visualDiff/view/detailsWidget/idListDiffWidget.h \

SOURCES += \
	$$PWD/visualDiff/diffPluginWrapper.cpp \
	$$PWD/visualDiff/model/diffModel.cpp \
	$$PWD/visualDiff/model/difference.cpp \
	$$PWD/visualDiff/model/modification.cpp \
	$$PWD/visualDiff/model/graphicalDifference.cpp \
	$$PWD/visualDiff/model/logicalDifference.cpp \
	$$PWD/visualDiff/model/commonDifference.cpp \
	$$PWD/visualDiff/model/modelLoader.cpp \
	$$PWD/visualDiff/view/diffColorPreferencesDialog.cpp \
	$$PWD/visualDiff/view/diffWindow.cpp \
	$$PWD/visualDiff/view/diffView.cpp \
	$$PWD/visualDiff/view/diffScene.cpp \
	$$PWD/visualDiff/view/detailsWidget/diffDetailsWidget.cpp \
	$$PWD/visualDiff/view/detailsWidget/idWidget.cpp \
	$$PWD/visualDiff/view/detailsWidget/parentWidget.cpp \
	$$PWD/visualDiff/view/detailsWidget/childrenWidget.cpp \
	$$PWD/visualDiff/view/detailsWidget/idListWidget.cpp \
	$$PWD/visualDiff/view/detailsWidget/propertiesWidget.cpp \
	$$PWD/visualDiff/view/detailsWidget/propertyWidget.cpp \
	$$PWD/visualDiff/view/detailsWidget/idListDiffWidget.cpp \

FORMS += \
    $$PWD/visualDiff/view/diffColorPreferencesDialog.ui \
