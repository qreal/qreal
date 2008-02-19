QT		+=	svg

INCLUDEPATH	+=	../qrxml/editors \
			../qrxml \
			mainwindow

RESOURCES	=	qrgui.qrc
SOURCES		=	main.cpp \
			mainwindow/mainwindow.cpp \
			mainwindow/palettetoolbox.cpp \
			dialogs/plugindialog.cpp

HEADERS		= \
			mainwindow/mainwindow.h \
			mainwindow/palettetoolbox.h \
			dialogs/plugindialog.h

FORMS		= \
			mainwindow/mainwindow.ui
