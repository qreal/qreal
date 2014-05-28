TEMPLATE = lib
CONFIG +=  plugin c++11 debug
DESTDIR = D:/qrealGen/bin/plugins/
OBJECTS_DIR = .obj
MOC_DIR = .moc

LIBS += -LD:/qrealGen/bin -lqrkernel -lqrutils -lqrrepo

QT += widgets

INCLUDEPATH += \
	D:/qrealGen \
	D:/qrealGen/qrgui/ \

HEADERS += \
	generator.h \
	qUbiqPresentationGeneratorPlugin.h \

SOURCES += \
	generator.cpp \
	qUbiqPresentationGeneratorPlugin.cpp \

win32 {
	QMAKE_POST_LINK = "xcopy templates D:\\qrealGen\\bin\\templates\\qUbiqPresentationGenerator /s /e /q /y /i "
}
else {
	QMAKE_POST_LINK = "mkdir D:/qrealGen/bin/templates/qUbiqPresentationGenerator && cp -r templates D:/qrealGen/bin/templates/qUbiqPresentationGenerator "
}