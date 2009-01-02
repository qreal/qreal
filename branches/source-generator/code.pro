QT = core \
    xml \
    svg \
    gui
TEMPLATE = app
DEPENDPATH += .
INCLUDEPATH += .

# Input
HEADERS += generator.h \
    entity.h
SOURCES += main.cpp \
    generator.cpp \
    entity.cpp
