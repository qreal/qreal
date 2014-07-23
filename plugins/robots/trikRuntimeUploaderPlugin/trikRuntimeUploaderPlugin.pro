QT += widgets
TEMPLATE = lib
CONFIG += plugin c++11
DESTDIR = ../../../bin/plugins/
MOC_DIR = .moc
RCC_DIR = .moc

INCLUDEPATH += \
	$$PWD/../../../ \
	$$PWD/../../../qrgui \

LIBS += -L../../../bin -lqrkernel -lqrutils -lqrrepo

win32 {
	QMAKE_POST_LINK = "cmd /C "xcopy trikRuntime ..\\..\\..\\bin\\trikRuntime /s /e /q /y /i""
}
else {
	QMAKE_POST_LINK = "mkdir ../../../bin/trikRuntime;"\
						"cp -r trikRuntime/* ../../../bin/trikRuntime/" \
}

HEADERS += \
	trikRuntimeUploaderPlugin.h

SOURCES += \
	trikRuntimeUploaderPlugin.cpp
