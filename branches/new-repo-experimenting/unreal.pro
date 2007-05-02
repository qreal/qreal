QT		+= sql svg
INCLUDEPATH	+= view umllib repo dialogs mainwindow umllib/generated
RESOURCES	+= real_dynamic.qrc static.qrc
SOURCES		= main.cpp


# SQL model
HEADERS         += dialogs/qsqlconnectiondialog.h \
		    repo/realrepomodel.h \
    		    repo/realrepoitem.h \
		    repo/realreporoles.h \
		    repo/realreponames.h
SOURCES	        += dialogs/qsqlconnectiondialog.cpp \
		    repo/realrepomodel.cpp \
		    repo/realrepoitem.cpp \
		    repo/realreporoles.cpp \
		    repo/realreponames.cpp \
		    repo/edges_stuff.cpp
FORMS           += dialogs/qsqlconnectiondialog.ui

# Mainwindow
HEADERS		+= mainwindow/mainwindow.h \
		   mainwindow/categoryfilterproxymodel.h
SOURCES		+= mainwindow/mainwindow.cpp \
		   mainwindow/categoryfilterproxymodel.cpp
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
