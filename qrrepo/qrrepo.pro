DESTDIR = ../bin

TEMPLATE = lib

OBJECTS_DIR = .obj
MOC_DIR = .moc

LIBS += -L../bin -lqrkernel -lqrutils

!macx {
	QMAKE_LFLAGS="-Wl,-O1,-rpath,$(PWD)"
}


include(qrrepo.pri)
