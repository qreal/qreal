QT += widgets

INCLUDEPATH += \
	$$PWD/../../interpreterBase/include \
	$$PWD/../../../utils/include \
	$$PWD/../../../../../ \
	$$PWD/../../../../../qrgui/ \

LIBS += -L$$PWD/../../../../../bin -linterpreterBase

HEADERS += \
	$$PWD/nullKitInterpreterPlugin.h \
	$$PWD/nullRobotModel.h \

SOURCES += \
	$$PWD/nullKitInterpreterPlugin.cpp \
	$$PWD/nullRobotModel.cpp \

TRANSLATIONS = $$PWD/../nullKitInterpreter_ru.ts

RESOURCES += $$PWD/../nullKitInterpreter.qrc
