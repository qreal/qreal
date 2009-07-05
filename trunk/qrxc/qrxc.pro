QT = core \
     xml \
     gui

CONFIG = qt debug

include ( qrxc.pri )

TARGET = $$QMAKE_QRXC

HEADERS += generator.h \
    entity.h \
    editor_file.h \
    editor.h \
    non_graph_type.h \
    property.h \
    sdftocpp.h

SOURCES += generator.cpp \
    entity.cpp \
    editor_file.cpp \
    editor.cpp \
    non_graph_type.cpp \
    property.cpp \
    sdftocpp.cpp \
    qrxc.cpp
