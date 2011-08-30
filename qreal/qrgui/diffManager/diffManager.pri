
HEADERS += \
	diffManager/diffManager.h \
	diffManager/details/diffProvider.h \
	diffManager/details/difference.h \
	diffManager/details/diffState.h \
	diffManager/details/modification.h \
	diffManager/diffView/diffView.h \
	diffManager/diffView/diffColorPreferencesDialog.h \
	diffManager/diffView/diffForm.h \
	diffManager/diffView/diffScene.h \
	diffManager/diffView/detailsWidget/diffDetailsWidget.h \
	diffManager/diffView/detailsWidget/idWidget.h \
	diffManager/diffView/detailsWidget/parentWidget.h \
	diffManager/details/graphicalDifference.h \
	diffManager/details/logicalDifference.h \
	diffManager/details/commonDifference.h \
	diffManager/details/elementType.h \
	diffManager/diffView/detailsWidget/childrenWidget.h \
	diffManager/diffView/detailsWidget/idListWidget.h \
	diffManager/diffView/detailsWidget/propertiesWidget.h \
	diffManager/diffView/detailsWidget/propertyWidget.h \
	diffManager/diffView/detailsWidget/idListDiffWidget.h

SOURCES += \
	diffManager/diffManager.cpp \
	diffManager/details/diffProvider.cpp \
	diffManager/details/difference.cpp \
	diffManager/details/modification.cpp \
	diffManager/diffView/diffView.cpp \
	diffManager/diffView/diffColorPreferencesDialog.cpp \
	diffManager/diffView/diffForm.cpp \
	diffManager/diffView/diffScene.cpp \
	diffManager/diffView/detailsWidget/diffdetailsWidget.cpp \
	diffManager/diffView/detailsWidget/idWidget.cpp \
	diffManager/diffView/detailsWidget/parentWidget.cpp \
	diffManager/details/graphicalDifference.cpp \
	diffManager/details/logicalDifference.cpp \
	diffManager/details/commonDifference.cpp \
	diffManager/diffView/detailsWidget/childrenWidget.cpp \
	diffManager/diffView/detailsWidget/idListWidget.cpp \
	diffManager/diffView/detailsWidget/propertiesWidget.cpp \
	diffManager/diffView/detailsWidget/propertyWidget.cpp \
	diffManager/diffView/detailsWidget/idListDiffWidget.cpp

FORMS += \
	diffManager/diffView/diffColorPreferencesDialog.ui
