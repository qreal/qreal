QT           += sql

HEADERS         = dialogs/qsqlconnectiondialog.h \
		    repo/realrepomodel.h \
		    repo/realrepoitem.h \
		    repo/realreponames.h
SOURCES         = dialogs/qsqlconnectiondialog.cpp \
		    repo/realrepomodel.cpp \
		    repo/realrepoitem.cpp \
		    repo/realreponames.cpp
FORMS           = dialogs/qsqlconnectiondialog.ui

HEADERS		+= mainwindow.h
SOURCES		+= mainwindow.cpp \
		    main.cpp

