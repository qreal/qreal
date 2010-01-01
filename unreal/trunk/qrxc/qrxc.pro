TEMPLATE = app
QT += xml gui
CONFIG += console
MOC_DIR = .moc

HEADERS += association.h \
    diagram.h \
    edgeType.h \
    editor.h \
    enumType.h \
    graphicType.h \
    label.h \
    linePort.h \
    nameNormalizer.h \
    nodeType.h \
    nonGraphicType.h \
    numericType.h \
    outFile.h \
    pointPort.h \
    port.h \
    property.h \
    scalableCoordinate.h \
    sdftocpp.h \
    stringType.h \
    type.h \
    xmlCompiler.h

SOURCES += association.cpp \
    diagram.cpp \
    edgeType.cpp \
    editor.cpp \
    enumType.cpp \
    graphicType.cpp \
    label.cpp \
    linePort.cpp \
    main.cpp \
    nameNormalizer.cpp \
    nodeType.cpp \
    nonGraphicType.cpp \
    numericType.cpp \
    outFile.cpp \
    pointPort.cpp \
    port.cpp \
    property.cpp \
    scalableCoordinate.cpp \
    sdftocpp.cpp \
    stringType.cpp \
    type.cpp \
    xmlCompiler.cpp