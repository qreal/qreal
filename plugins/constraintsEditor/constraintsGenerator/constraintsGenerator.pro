include(../../../global.pri)

QT += xml widgets

TEMPLATE = lib
CONFIG += plugin c++11
DESTDIR = $$DESTDIR/plugins/tools
MOC_DIR = .moc
RCC_DIR = .moc

INCLUDEPATH += \
	$$PWD/../../.. \
	$$PWD/../../../qrgui/ \

links(qrkernel qrutils qrmc)

TRANSLATIONS = $$PWD/../../../qrtranslations/ru/plugins/constraintsGenerator_ru.ts

HEADERS = \
	constraintsGeneratorPlugin.h \
	generator.h \
	concreteGenerator.h \
	generatorUtils/commonGeneratorInfo.h \
	generatorUtils/defs.h \

SOURCES = \
	constraintsGeneratorPlugin.cpp \
	generator.cpp \
	concreteGenerator.cpp \
	generatorUtils/commonGeneratorInfo.cpp \

win32 {
	QMAKE_POST_LINK = "xcopy templates ..\\..\\..\\bin\\templates /s /e /q /y /i "
}
else {
	QMAKE_POST_LINK = "mkdir ../../../bin/templates/ && cp -r templates ../../../bin/templates/ "
}







