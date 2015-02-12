QT += widgets

links(qrtext robots-interpreter-base)
includes(plugins/robots/interpreters/interpreterBase plugins/robots/utils qrtext)

HEADERS += \
	$$PWD/nullKitInterpreterPlugin.h \
	$$PWD/nullRobotModel.h \

SOURCES += \
	$$PWD/nullKitInterpreterPlugin.cpp \
	$$PWD/nullRobotModel.cpp \

TRANSLATIONS = $$PWD/../../../../../qrtranslations/ru/plugins/robots/nullKitInterpreter_ru.ts
