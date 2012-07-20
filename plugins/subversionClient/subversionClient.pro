TEMPLATE = lib
CONFIG += plugin
DESTDIR = ../../bin/plugins/

MOC_DIR = .moc
RCC_DIR = .rcc
UI_DIR = .ui
OBJECTS_DIR = .obj

TRANSLATIONS += subversionClient_ru.ts

HEADERS = \
	subversionPlugin.h \
	viewInteraction.h \
	gui/checkoutDialog.h \
	gui/commitDialog.h \
	gui/versioningPage.h \
	../../qrgui/dialogs/preferencesPages/preferencesPage.h \

SOURCES = \
	subversionPlugin.cpp \
	viewInteraction.cpp \
	gui/checkoutDialog.cpp \
	gui/commitDialog.cpp \
	gui/versioningPage.cpp \

FORMS = \
	gui/versioningPage.ui \
