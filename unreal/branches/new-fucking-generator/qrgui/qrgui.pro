QT		+=	svg xml

INCLUDEPATH	+=	../qrxml/editors \
			../qrxml \
			mainwindow

RESOURCES	=	qrgui.qrc
SOURCES		=	main.cpp

OBJECTS_DIR = .obj
MOC_DIR = .moc

if (equals(QMAKE_CXX, "g++")) {
    QMAKE_LFLAGS += -Wl,-E
}

# Graphical elements
include (umllib/umllib.pri)

# Dialogs
include (dialogs/dialogs.pri)

# Main window
include (mainwindow/mainwindow.pri)

# Client
include (client/client.pri)

# Model
include (model/model.pri)

# View
include (view/view.pri)

# Stuff used in all places
include (kernel/kernel.pri)

# "Встроенные" генераторы
include (generators/generators.pri)

HEADERS += ../qrxml/editorinterface.h