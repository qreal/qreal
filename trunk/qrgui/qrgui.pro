QT		+=	svg xml

INCLUDEPATH	+=	../qrxml/editors \
			../qrxml \
			mainwindow

RESOURCES	=	qrgui.qrc
SOURCES		=	main.cpp \
			mainwindow/mainwindow.cpp \
			mainwindow/palettetoolbox.cpp \
			mainwindow/editormanager.cpp \
			dialogs/plugindialog.cpp

HEADERS		= \
			mainwindow/mainwindow.h \
			mainwindow/palettetoolbox.h \
			mainwindow/editormanager.h \
			dialogs/plugindialog.h

FORMS		= \
			mainwindow/mainwindow.ui

# Graphical elements
include (umllib/umllib.pri)
