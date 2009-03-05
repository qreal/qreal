QT = core \
    xml

TEMPLATE = app
DEPENDPATH += .
INCLUDEPATH += .

# Input
HEADERS += generator.h \
    entity.h \
    editor_file.h \
    editor.h \
    property.h

SOURCES += main.cpp \
    generator.cpp \
    entity.cpp \
    editor_file.cpp \
    editor.cpp \
    property.cpp
