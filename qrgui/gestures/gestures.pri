HEADERS += \
	$$PWD/gesturesPainterInterface.h \
	$$PWD/mouseMovementManager.h \
	$$PWD/gesturesWidget.h \
	$$PWD/gesturePainter.h \
	$$PWD/private/geometricForms.h \
	$$PWD/private/pathCorrector.h \
	$$PWD/private/levenshteinDistance.h \
	$$PWD/private/keyManager.h \
	$$PWD/private/abstractRecognizer.h \
	$$PWD/private/mixedgesturesmanager.h \
	$$PWD/private/rectanglegesturesmanager.h \
	$$PWD/private/nearestposgridgesturesmanager.h \
	$$PWD/private/curveKeyBuilder.h \
	$$PWD/private/sorts.h \

SOURCES += \
	$$PWD/mouseMovementManager.cpp \
	$$PWD/gesturesWidget.cpp \
	$$PWD/gesturePainter.cpp \
	$$PWD/private/pathCorrector.cpp \
	$$PWD/private/levenshteinDistance.cpp \
	$$PWD/private/keyManager.cpp \
	$$PWD/private/mixedgesturesmanager.cpp \
	$$PWD/private/rectanglegesturesmanager.cpp \
	$$PWD/private/nearestposgridgesturesmanager.cpp \

FORMS += \
	$$PWD/gesturesWidget.ui \
