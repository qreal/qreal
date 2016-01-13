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
	generatorUtils/defs.h \
	generatorUtils/auxiliaryMethods.h \
	generatorParts/generatorForListsOfElements.h \
	generatorParts/generatorForProperties.h \
	generatorParts/generatorForNodeElements.h \
	generatorParts/generatorForElements.h \
	generatorParts/generatorForEdgeElements.h \
	generatorUtils/commonDataForAllGenerators.h \
	generatorParts/generatorForDiagrams.h \

SOURCES = \
	constraintsGeneratorPlugin.cpp \
	generator.cpp \
	concreteGenerator.cpp \
	generatorUtils/auxiliaryMethods.cpp \
	generatorParts/generatorForListsOfElements.cpp \
	generatorParts/generatorForProperties.cpp \
	generatorParts/generatorForNodeElements.cpp \
	generatorParts/generatorForElements.cpp \
	generatorParts/generatorForEdgeElements.cpp \
	generatorUtils/commonDataForAllGenerators.cpp \
	generatorParts/generatorForDiagrams.cpp \

win32 {
	QMAKE_POST_LINK = "xcopy templates ..\\..\\..\\bin\\templates /s /e /q /y /i "
}
else {
	QMAKE_POST_LINK = "mkdir ../../../bin/templates/ && cp -r templates ../../../bin/templates/ "
}
