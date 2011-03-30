# -------------------------------------------------
# Project created by QtCreator 2009-11-01T22:19:40
# -------------------------------------------------
QT += xml
TARGET = MouseGestures
TEMPLATE = app
SOURCES += main.cpp \
	mousegestures.cpp \
	pathcorrector.cpp \
	serializer.cpp \
	validpathcreator.cpp \
	adopter.cpp \
	paintmanager.cpp \
    test/testwindow.cpp \
    test/xmlparser.cpp \
    multistrokeRecognizers/sumMultistrokeGesturesManager.cpp \
    multistrokeRecognizers/rectanglegesturesmanager.cpp \
    multistrokeRecognizers/nearestposgridgesturesmanager.cpp
HEADERS += mousegestures.h \
	pathcorrector.h \
	serializer.h \
	GeometricForms.h \
	validpathcreator.h \
	adopter.h \
	paintmanager.h \
	figures.h \
	abstractRecognizer.h \
    test/testwindow.h \
    test/xmlparser.h \
    multistrokeRecognizers/sorts.h \
    multistrokeRecognizers/distance.h \
    multistrokeRecognizers/curveKeyBuilder.h \
    multistrokeRecognizers/multistrokeGesturesManagers.h \
    TrainingGesturesManager/TrainingGesturesManager.h \
    NeuralNetwork/neuralnetwork.h \
    test/testThread.h \
    multistrokeRecognizers/sumMultistrokeGesturesManager.h \
    multistrokeRecognizers/gridKeyBuilder.h \
    multistrokeRecognizers/rectanglegesturesmanager.h \
    multistrokeRecognizers/nearestposgridgesturesmanager.h
FORMS += mousegestures.ui \
    test/testwindow.ui
