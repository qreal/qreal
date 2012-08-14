TEMPLATE = lib
CONFIG += plugin
DESTDIR = ../../bin/plugins/

MOC_DIR = .moc
RCC_DIR = .moc
UI_DIR = .ui
OBJECTS_DIR = .obj

LIBS += -L../../bin -lqrkernel

HEADERS += \
	diffPlugin.h \
	model/diffModel.h \
	model/difference.h \
	model/diffState.h \
	model/modification.h \
	model/graphicalDifference.h \
	model/logicalDifference.h \
	model/commonDifference.h \
	model/elementType.h \
	model/modelLoader.h \
	view/diffColorPreferencesDialog.h \
	view/diffWindow.h \
	view/diffViewHelper.h \
	view/diffSceneHelper.h \
	view/detailsWidget/diffDetailsWidget.h \
	view/detailsWidget/idWidget.h \
	view/detailsWidget/parentWidget.h \
	view/detailsWidget/childrenWidget.h \
	view/detailsWidget/idListWidget.h \
	view/detailsWidget/propertiesWidget.h \
	view/detailsWidget/propertyWidget.h \
	view/detailsWidget/idListDiffWidget.h \

SOURCES += \
	diffPlugin.cpp \
	model/diffModel.cpp \
	model/difference.cpp \
	model/modification.cpp \
	model/graphicalDifference.cpp \
	model/logicalDifference.cpp \
	model/commonDifference.cpp \
	model/modelLoader.cpp \
	view/diffColorPreferencesDialog.cpp \
	view/diffWindow.cpp \
	view/diffViewHelper.cpp \
	view/diffSceneHelper.cpp \
	view/detailsWidget/diffDetailsWidget.cpp \
	view/detailsWidget/idWidget.cpp \
	view/detailsWidget/parentWidget.cpp \
	view/detailsWidget/childrenWidget.cpp \
	view/detailsWidget/idListWidget.cpp \
	view/detailsWidget/propertiesWidget.cpp \
	view/detailsWidget/propertyWidget.cpp \
	view/detailsWidget/idListDiffWidget.cpp \

FORMS += \
	view/diffColorPreferencesDialog.ui \
