QT += xml

DESTDIR = ../bin

TEMPLATE = lib

OBJECTS_DIR = .obj
MOC_DIR = .moc

LIBS += -L../bin -lqrkernel -lqrutils

!macx {
	QMAKE_LFLAGS="-Wl,-O1,-rpath,$(PWD)"
}

DEFINES += QRREPO_LIBRARY

include(qrrepo.pri)
