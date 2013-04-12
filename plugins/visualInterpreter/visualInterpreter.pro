QT += xml

TEMPLATE = lib
CONFIG += plugin
DESTDIR = ../../bin/plugins/

OBJECTS_DIR = .obj
MOC_DIR = .moc
RCC_DIR = .moc

LIBS += -L../../bin -lqrkernel -lqrutils

TRANSLATIONS = visualInterpreter_ru.ts

HEADERS = \
	../../qrgui/dialogs/preferencesPages/preferencesPage.h \
	visualInterpreterPlugin.h \
	visualInterpreterPreferencesPage.h \
	ruleParser.h \
	visualInterpreterUnit.h \
	pythonInterpreter.h \
	pythonGenerator.h

SOURCES = \
	visualInterpreterPlugin.cpp \
	visualInterpreterPreferencesPage.cpp \
	ruleParser.cpp \
	visualInterpreterUnit.cpp \
	pythonInterpreter.cpp \
	pythonGenerator.cpp

FORMS += \
	visualInterpreterPreferencePage.ui \

RESOURCES += \
	visualInterpreter.qrc \
