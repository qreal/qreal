TEMPLATE = lib

DESTDIR = $$PWD/../../bin

QT += widgets

LIBS += -L$$PWD/../../bin -lqrkernel -lqrutils -lmodels

INCLUDEPATH = \
	$$PWD/ \
	$$PWD/../ \
	$$PWD/../../ \

CONFIG += c++11

OBJECTS_DIR = .obj
UI_DIR = .ui
MOC_DIR = .moc
RCC_DIR = .moc

DEFINES += QRGUI_DIALOGS_LIBRARY

HEADERS += \
	$$PWD/dialogsDeclSpec.h \
	$$PWD/metamodelingOnFly/propertiesDialog.h \
	$$PWD/metamodelingOnFly/editPropertiesDialog.h \
	$$PWD/metamodelingOnFly/chooseTypeDialog.h \
	$$PWD/metamodelingOnFly/edgePropertiesDialog.h \
	$$PWD/metamodelingOnFly/addNodeDialog.h \
	$$PWD/metamodelingOnFly/restorePropertiesDialog.h \
	$$PWD/metamodelingOnFly/restoreElementDialog.h \
	$$PWD/progressDialog/progressBar.h \
	$$PWD/progressDialog/progressDialog.h \
	$$PWD/projectManagement/listWidget.h \
	$$PWD/projectManagement/managedClosableDialog.h \
	$$PWD/projectManagement/suggestToCreateDiagramWidget.h \
	$$PWD/projectManagement/suggestToCreateDiagramDialog.h \
	$$PWD/projectManagement/suggestToCreateProjectDialog.h \
	$$PWD/findReplaceDialog.h \

SOURCES += \
	$$PWD/metamodelingOnFly/propertiesDialog.cpp \
	$$PWD/metamodelingOnFly/editPropertiesDialog.cpp \
	$$PWD/metamodelingOnFly/chooseTypeDialog.cpp \
	$$PWD/metamodelingOnFly/edgePropertiesDialog.cpp \
	$$PWD/metamodelingOnFly/addNodeDialog.cpp \
	$$PWD/metamodelingOnFly/restorePropertiesDialog.cpp \
	$$PWD/metamodelingOnFly/restoreElementDialog.cpp \
	$$PWD/progressDialog/progressBar.cpp \
	$$PWD/progressDialog/progressDialog.cpp \
	$$PWD/projectManagement/listWidget.cpp \
	$$PWD/projectManagement/managedClosableDialog.cpp \
	$$PWD/projectManagement/suggestToCreateDiagramWidget.cpp \
	$$PWD/projectManagement/suggestToCreateDiagramDialog.cpp \
	$$PWD/projectManagement/suggestToCreateProjectDialog.cpp \
	$$PWD/findReplaceDialog.cpp \

FORMS += \
	$$PWD/metamodelingOnFly/propertiesDialog.ui \
	$$PWD/metamodelingOnFly/editPropertiesDialog.ui \
	$$PWD/metamodelingOnFly/chooseTypeDialog.ui \
	$$PWD/metamodelingOnFly/edgePropertiesDialog.ui \
	$$PWD/metamodelingOnFly/addNodeDialog.ui \
	$$PWD/metamodelingOnFly/restorePropertiesDialog.ui \
	$$PWD/metamodelingOnFly/restoreElementDialog.ui \
	$$PWD/findReplaceDialog.ui \
