include(../../../global.pri)

TEMPLATE = lib
CONFIG += plugin 

DESTDIR = $$DESTDIR/plugins/tools/

links(qrkernel qslog qrgui-preferences-dialog)

INCLUDEPATH += \
	$$PWD/../../../ \
	$$PWD/../../../qrgui/ \

QT += widgets xml

TRANSLATIONS = $$PWD/../../../qrtranslations/ru/plugins/updatesChecker_ru.ts

HEADERS = \
	$$PWD/updatesCheckerPlugin.h \
	$$PWD/updater.h \
	$$PWD/updateVersionDialog.h \

SOURCES = \
	$$PWD/updatesCheckerPlugin.cpp \
	$$PWD/updater.cpp \
	$$PWD/updateVersionDialog.cpp \

RESOURCES += \
	$$PWD/updatesChecker.qrc \
