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

HEADERS += \
	$$PWD/metamodelingOnFly/propertiesDialog.h \
	$$PWD/metamodelingOnFly/editPropertiesDialog.h \
	$$PWD/metamodelingOnFly/chooseTypeDialog.h \
	$$PWD/metamodelingOnFly/edgePropertiesDialog.h \
	$$PWD/metamodelingOnFly/addNodeDialog.h \
	$$PWD/metamodelingOnFly/restorePropertiesDialog.h \
	$$PWD/metamodelingOnFly/restoreElementDialog.h \
	$$PWD/progressDialog/progressBar.h \
	$$PWD/progressDialog/progressDialog.h \
	$$PWD/findReplaceDialog.h \
	$$PWD/managedClosableDialog.h \
	$$PWD/suggestToCreateDiagramDialog.h \
	$$PWD/suggestToCreateProjectDialog.h \

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
	$$PWD/findReplaceDialog.cpp \
	$$PWD/managedClosableDialog.cpp \
	$$PWD/suggestToCreateDiagramDialog.cpp \
	$$PWD/suggestToCreateProjectDialog.cpp \

FORMS += \
	$$PWD/metamodelingOnFly/propertiesDialog.ui \
	$$PWD/metamodelingOnFly/editPropertiesDialog.ui \
	$$PWD/metamodelingOnFly/chooseTypeDialog.ui \
	$$PWD/metamodelingOnFly/edgePropertiesDialog.ui \
	$$PWD/metamodelingOnFly/addNodeDialog.ui \
	$$PWD/metamodelingOnFly/restorePropertiesDialog.ui \
	$$PWD/metamodelingOnFly/restoreElementDialog.ui \
	$$PWD/findReplaceDialog.ui \
