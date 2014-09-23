QT += widgets

INCLUDEPATH += \
	$$PWD/../../interpreterBase/include \
	$$PWD/../../../utils/include \
	$$PWD/../../../../../ \
	$$PWD/../../../../../qrgui/ \
	$$PWD/../../../../../qrtext/include \

LIBS += -L$$PWD/../../../../../bin -linterpreterBase

HEADERS += \
	$$PWD/ev3KitInterpreterPlugin.h \
	$$PWD/ev3RobotModel.h \

SOURCES += \
	$$PWD/ev3KitInterpreterPlugin.cpp \
	$$PWD/ev3RobotModel.cpp \

TRANSLATIONS = $$PWD/../../../../../qrtranslations/ru/plugins/robots/ev3KitInterpreter_ru.ts
