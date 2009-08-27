HEADERS += umllib/uml_edgeelement.h \
    umllib/uml_element.h \
    umllib/uml_guiobjectfactory.h \
    umllib/uml_nodeelement.h \
    umllib/sdfrenderer.h \
    umllib/pathCreator.h
SOURCES += umllib/uml_edgeelement.cpp \
    umllib/uml_element.cpp \
    umllib/uml_nodeelement.cpp \
    umllib/sdfrenderer.cpp \
    umllib/pathCreator.cpp \
    umllib/uml_guiobjectfactory.cpp

include (umllib/generated/umllib.pri)
