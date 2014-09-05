QT += xml script widgets

TEMPLATE = lib
CONFIG += plugin c++11

DESTDIR = ../../bin/plugins/

OBJECTS_DIR = .obj
MOC_DIR = .moc
RCC_DIR = .moc

LIBS += -L../../bin -lqrkernel -lqrutils

INCLUDEPATH += \
	$$PWD/../.. \
	$$PWD/../../qrgui/ \

TRANSLATIONS = $$PWD/../../qrtranslations/ru/plugins/visualInterpreter_ru.ts

HEADERS = \
	../../qrgui/dialogs/preferencesPages/preferencesPage.h \
	visualInterpreterPlugin.h \
	visualInterpreterPreferencesPage.h \
	visualInterpreterUnit.h \
	textualPart/ruleParser.h \
	textualPart/pythonInterpreter.h \
	textualPart/pythonGenerator.h \
	textualPart/textCodeGenerator.h \
	textualPart/textCodeInterpreter.h \
	textualPart/qtScriptGenerator.h \
	textualPart/qtScriptInterpreter.h

SOURCES = \
	visualInterpreterPlugin.cpp \
	visualInterpreterPreferencesPage.cpp \
	visualInterpreterUnit.cpp \
	textualPart/ruleParser.cpp \
	textualPart/pythonInterpreter.cpp \
	textualPart/pythonGenerator.cpp \
	textualPart/textCodeGenerator.cpp \
	textualPart/textCodeInterpreter.cpp \
	textualPart/qtScriptGenerator.cpp \
	textualPart/qtScriptInterpreter.cpp

FORMS += \
	visualInterpreterPreferencePage.ui \

RESOURCES += \
	visualInterpreter.qrc \
