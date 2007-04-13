QT           += sql

INCLUDEPATH	+= view umllib repo dialogs

# SQL model
HEADERS         = dialogs/qsqlconnectiondialog.h \
		    repo/realrepomodel.h \
		    repo/realrepoitem.h \
		    repo/realreponames.h
SOURCES         = dialogs/qsqlconnectiondialog.cpp \
		    repo/realrepomodel.cpp \
		    repo/realrepoitem.cpp \
		    repo/realreponames.cpp
FORMS           = dialogs/qsqlconnectiondialog.ui

# View
HEADERS			+= view/editorview.h \
			view/editorviewscene.h \
			view/editorviewmviface.h
SOURCES			+= view/editorview.cpp \
			view/editorviewscene.cpp \
			view/editorviewmviface.cpp

# UML library
HEADERS		+= umllib/uml_edgeelement.h \
			umllib/uml_element.h \
			umllib/uml_glamour_class.h \
			umllib/uml_guiobjectfactory.h \
			umllib/uml_nodeelement.h
SOURCES		+= umllib/uml_edgeelement.cpp \
			umllib/uml_element.cpp \
			umllib/uml_glamour_class.cpp \
			umllib/uml_guiobjectfactory.cpp \
			umllib/uml_nodeelement.cpp


HEADERS		+= mainwindow.h
SOURCES		+= mainwindow.cpp \
		    main.cpp

