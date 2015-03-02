include(../../../global.pri)

QT += xml script widgets

TEMPLATE = lib
CONFIG += plugin

DESTDIR = $$DESTDIR/plugins/tools/

links(qrkernel qrutils qrgui-preferences-dialog)

INCLUDEPATH += \
	$$PWD/../../.. \
	$$PWD/../../../qrgui/ \
	$$PWD/../../../qrtext/include/ \

TRANSLATIONS = $$PWD/../../../qrtranslations/ru/plugins/visualInterpreter_ru.ts

HEADERS = \
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
