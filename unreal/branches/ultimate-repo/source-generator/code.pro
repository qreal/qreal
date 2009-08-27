QT = core \
    xml \
    gui
TEMPLATE = app
DEPENDPATH += .
INCLUDEPATH += .

# Input
HEADERS += generator.h \
    entity.h \
    editor_file.h \
    editor.h \
    non_graph_type.h \
    property.h \
    sdftocpp.h
SOURCES += main.cpp \
    generator.cpp \
    entity.cpp \
    editor_file.cpp \
    editor.cpp \
    non_graph_type.cpp \
    property.cpp \
    sdftocpp.cpp
