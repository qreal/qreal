QT		+= sql svg opengl network 
CONFIG += qt debug
INCLUDEPATH	+= view umllib repo dialogs mainwindow umllib/generated
RESOURCES	+= real_dynamic.qrc static.qrc
SOURCES		= main.cpp

OBJECTS_DIR 	= obj
MOC_DIR		= moc

# Model
HEADERS         += dialogs/qsqlconnectiondialog.h \
		    repo/realrepomodel.h \
		    repo/realrepoclient.h \
		    common/realrepoapiclasses.h \
		    repo/realrepoinfo.h \
		    common/classes.h

SOURCES	        += dialogs/qsqlconnectiondialog.cpp \
		    repo/realrepomodel.cpp \
		    common/realrepoapiclasses.cpp \
		    repo/realrepoclient.cpp \
		    repo/realrepoinfo.cpp \
		    common/classes.cpp

FORMS           += dialogs/qsqlconnectiondialog.ui

# Mainwindow
HEADERS		+= mainwindow/mainwindow.h \
		   mainwindow/categoryfilterproxymodel.h \
                   mainwindow/propertyeditorproxymodel.h \
		   mainwindow/palettetoolbox.h
SOURCES		+= mainwindow/mainwindow.cpp \
		   mainwindow/categoryfilterproxymodel.cpp \
                   mainwindow/propertyeditorproxymodel.cpp \
		   mainwindow/palettetoolbox.cpp
FORMS		+= mainwindow/mainwindow.ui

# View
HEADERS		+= view/editorview.h \
    		   view/editorviewscene.h \
		   view/editorviewmviface.h
SOURCES		+= view/editorview.cpp \
	           view/editorviewscene.cpp \
	           view/editorviewmviface.cpp

# UML library
HEADERS 	+= umllib/uml_edgeelement.h \
	           umllib/uml_element.h \
		   umllib/uml_guiobjectfactory.h \
		   umllib/uml_nodeelement.h
SOURCES		+= umllib/uml_edgeelement.cpp \
		   umllib/uml_element.cpp \
		   umllib/uml_guiobjectfactory.cpp \
		   umllib/uml_nodeelement.cpp

# Generated UML code
include (umllib/generated/umllib.pri)
SOURCES		+= repo/edges_stuff.cpp
