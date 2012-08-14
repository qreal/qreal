HEADERS += \
	versioning/versioningPluginsManager.h \
	versioning/versioningPluginInterface.h \
	versioning/briefVersioningInterface.h \
	versioning/diffPluginBase.h \
	versioning/diffPluginInterface.h \

SOURCES += \
	versioning/versioningPluginsManager.cpp \
	versioning/diffPluginBase.cpp \

# Visual diff
# This a temp solution till system would be refactored
#  the components could be reused in plugins
HEADERS += \
	versioning/visualDiff/diffPluginWrapper.h \
	versioning/visualDiff/model/diffModel.h \
	versioning/visualDiff/model/difference.h \
	versioning/visualDiff/model/diffState.h \
	versioning/visualDiff/model/modification.h \
	versioning/visualDiff/model/graphicalDifference.h \
	versioning/visualDiff/model/logicalDifference.h \
	versioning/visualDiff/model/commonDifference.h \
	versioning/visualDiff/model/elementType.h \
	versioning/visualDiff/model/modelLoader.h \
	versioning/visualDiff/view/diffColorPreferencesDialog.h \
	versioning/visualDiff/view/diffWindow.h \
	versioning/visualDiff/view/diffView.h \
	versioning/visualDiff/view/diffScene.h \
	versioning/visualDiff/view/detailsWidget/diffDetailsWidget.h \
	versioning/visualDiff/view/detailsWidget/idWidget.h \
	versioning/visualDiff/view/detailsWidget/parentWidget.h \
	versioning/visualDiff/view/detailsWidget/childrenWidget.h \
	versioning/visualDiff/view/detailsWidget/idListWidget.h \
	versioning/visualDiff/view/detailsWidget/propertiesWidget.h \
	versioning/visualDiff/view/detailsWidget/propertyWidget.h \
	versioning/visualDiff/view/detailsWidget/idListDiffWidget.h \

SOURCES += \
	versioning/visualDiff/diffPluginWrapper.cpp \
	versioning/visualDiff/model/diffModel.cpp \
	versioning/visualDiff/model/difference.cpp \
	versioning/visualDiff/model/modification.cpp \
	versioning/visualDiff/model/graphicalDifference.cpp \
	versioning/visualDiff/model/logicalDifference.cpp \
	versioning/visualDiff/model/commonDifference.cpp \
	versioning/visualDiff/model/modelLoader.cpp \
	versioning/visualDiff/view/diffColorPreferencesDialog.cpp \
	versioning/visualDiff/view/diffWindow.cpp \
	versioning/visualDiff/view/diffView.cpp \
	versioning/visualDiff/view/diffScene.cpp \
	versioning/visualDiff/view/detailsWidget/diffDetailsWidget.cpp \
	versioning/visualDiff/view/detailsWidget/idWidget.cpp \
	versioning/visualDiff/view/detailsWidget/parentWidget.cpp \
	versioning/visualDiff/view/detailsWidget/childrenWidget.cpp \
	versioning/visualDiff/view/detailsWidget/idListWidget.cpp \
	versioning/visualDiff/view/detailsWidget/propertiesWidget.cpp \
	versioning/visualDiff/view/detailsWidget/propertyWidget.cpp \
	versioning/visualDiff/view/detailsWidget/idListDiffWidget.cpp \

FORMS += \
	versioning/visualDiff/view/diffColorPreferencesDialog.ui \
