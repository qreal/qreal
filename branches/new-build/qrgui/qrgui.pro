QT		+=	svg

INCLUDEPATH	+=	../qrxml/editors \
			../qrxml

RESOURCES	=	qrgui.qrc

SOURCES		= \
			main.cpp \
			mainwindow/mainwindow.cpp \
			dialogs/plugindialog.cpp

HEADERS		= \
			mainwindow/mainwindow.h \
			dialogs/plugindialog.h

FORMS		= \
			mainwindow/mainwindow.ui
