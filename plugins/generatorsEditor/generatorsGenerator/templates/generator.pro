TEMPLATE = lib
CONFIG +=  plugin c++11
DESTDIR = @@pathToQReal@@/bin/plugins/
OBJECTS_DIR = .obj
MOC_DIR = .moc

LIBS += -L@@pathToQReal@@/bin -lqrkernel -lqrutils -lqrrepo

QT += widgets

INCLUDEPATH += \
	@@pathToQReal@@ \
	@@pathToQReal@@/qrgui/ \

HEADERS += \
	generator.h \
	@@generatorName@@Plugin.h \

SOURCES += \
	generator.cpp \
	@@generatorName@@Plugin.cpp \

win32 {
	QMAKE_POST_LINK = "xcopy templates @@strangePathToQReal@@\\bin\\templates\\@@generatorName@@ /s /e /q /y /i "
}
else {
	QMAKE_POST_LINK = "mkdir @@pathToQReal@@/bin/templates/@@generatorName@@ && cp -r templates @@pathToQReal@@/bin/templates/@@generatorName@@ "
}