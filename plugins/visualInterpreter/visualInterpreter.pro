QT += xml

TEMPLATE = lib
CONFIG += plugin
DESTDIR = ../../bin/plugins/
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

SOURCES = \
	visualInterpreterPlugin.cpp \
	visualInterpreterPreferencesPage.cpp \
	ruleParser.cpp \
	visualInterpreterUnit.cpp \

FORMS += \
	visualInterpreterPreferencePage.ui \

RESOURCES += \
	visualInterpreter.qrc \
