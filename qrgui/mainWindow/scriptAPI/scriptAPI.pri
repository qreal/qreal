QT += script testlib
QT.testlib.CONFIG -= console

#методы внутри некоторых классов делятся условно на функциональные методы и методы проверки. например:
#Q_INVOKABLE QRect propertyRect(const QString &name) const; - получаем QRect, по которому будем кликать для изменения свойства name.
#Q_INVOKABLE QString propertyValue(const QString &name) const; - получаем нужную строку, которую будем сравнивать с записанной или как-либо измененной.


HEADERS += \
	$$PWD/scriptAPI.h \
	$$PWD/arrow.h \
	$$PWD/virtualCursor.h \
	$$PWD/guiFacade.h \
	$$PWD/hintReporter.h \
	$$PWD/paletteAPI.h \
	$$PWD/hintAPI.h \
	$$PWD/virtualKeyboard.h \
	$$PWD/sceneAPI.h \
	$$PWD/scriptRegisterMetaTypes.h \

SOURCES += \
	$$PWD/scriptAPI.cpp \
	$$PWD/arrow.cpp \
	$$PWD/virtualCursor.cpp \
	$$PWD/guiFacade.cpp \
	$$PWD/hintReporter.cpp \
	$$PWD/paletteAPI.cpp \
	$$PWD/hintAPI.cpp \
	$$PWD/virtualKeyboard.cpp \
	$$PWD/sceneAPI.cpp \
