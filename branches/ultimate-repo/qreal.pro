QT += sql \
    svg \
    opengl \
    network \
    xml

# CONFIG += qt debug

INCLUDEPATH += view \
    umllib \
    repo \
    dialogs \
    mainwindow \
    XmiHandler \
    umllib/generated
RESOURCES += static.qrc real_dynamic.qrc
SOURCES = main.cpp
OBJECTS_DIR = .obj
MOC_DIR = .moc

# Model
include (repo/repo.pri)

# Common
include (common/common.pri)

# Dialogs
include (dialogs/dialogs.pri)

# Mainwindow
include (mainwindow/mainwindow.pri)

# View
include (view/view.pri)

# UML library
include (umllib/umllib.pri)

# Xmi Import_Export
include (xmiHandler/xmiHandler.pri)
