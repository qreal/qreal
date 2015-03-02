include(../../../../global.pri)

QT += xml

TEMPLATE = lib
CONFIG += plugin 
DESTDIR = $$DESTDIR/plugins/tools/

links(qrkernel qrutils qrrepo qrgui-preferences-dialog)

INCLUDEPATH += \
	$$PWD/../../../.. \
	$$PWD/../../../../qrgui/ \

QT += widgets

TRANSLATIONS = $$PWD/../../../../qrtranslations/ru/plugins/refactoringSupport_ru.ts

HEADERS = \
	refactoringPlugin.h \
	refactoringWindow.h \
	refactoringPreferencePage.h \
	refactoringFinder.h \
	refactoringApplier.h \


SOURCES = \
	refactoringPlugin.cpp \
	refactoringWindow.cpp \
	refactoringPreferencePage.cpp \
	refactoringFinder.cpp \
	refactoringApplier.cpp \


FORMS += \
	refactoringWindow.ui \
	refactoringPreferencePage.ui \

RESOURCES += \
	refactoring.qrc
