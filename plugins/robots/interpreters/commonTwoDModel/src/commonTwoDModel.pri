QT += widgets xml

INCLUDEPATH += \
	$$PWD/../include/ \
	$$PWD/../../../utils/include/ \
	$$PWD/../../../../../ \

LIBS += -L$$PWD/../../../../../bin/ -lqrkernel -lqrutils -lutils -linterpreterBase

HEADERS += \
	$$PWD/../include/commonTwoDModel/engine/configurer.h \

HEADERS += \
	$$PWD/engine/d2ModelWidget.h \
	$$PWD/engine/d2ModelScene.h \
	$$PWD/engine/d2RobotModel.h \
	$$PWD/engine/robotItem.h \
	$$PWD/engine/sensorItem.h \
	$$PWD/engine/rotater.h \
	$$PWD/engine/sonarSensorItem.h \
	$$PWD/engine/robotModelInterface.h \
	$$PWD/engine/sensorsConfiguration.h \
	$$PWD/engine/worldModel.h \
	$$PWD/engine/wallItem.h \
	$$PWD/engine/stylusItem.h \
	$$PWD/engine/lineItem.h \
	$$PWD/engine/colorFieldItem.h \
	$$PWD/engine/ellipseItem.h \
	$$PWD/engine/timeline.h \
	$$PWD/engine/d2ModelTimer.h \
	$$PWD/engine/gridParameters.h \
	$$PWD/engine/constants.h \
	$$PWD/engine/physics/physicsEngineBase.h \
	$$PWD/engine/physics/simplePhysicsEngine.h \
	$$PWD/engine/physics/realisticPhysicsEngine.h \

SOURCES += \
	$$PWD/engine/d2ModelWidget.cpp \
	$$PWD/engine/d2ModelScene.cpp \
	$$PWD/engine/d2RobotModel.cpp \
	$$PWD/engine/robotItem.cpp \
	$$PWD/engine/sensorItem.cpp \
	$$PWD/engine/rotater.cpp \
	$$PWD/engine/sonarSensorItem.cpp \
	$$PWD/engine/sensorsConfiguration.cpp \
	$$PWD/engine/worldModel.cpp \
	$$PWD/engine/wallItem.cpp \
	$$PWD/engine/stylusItem.cpp \
	$$PWD/engine/lineItem.cpp \
	$$PWD/engine/colorFieldItem.cpp \
	$$PWD/engine/ellipseItem.cpp \
	$$PWD/engine/timeline.cpp \
	$$PWD/engine/d2ModelTimer.cpp \
	$$PWD/engine/gridParameters.cpp \
	$$PWD/engine/physics/physicsEngineBase.cpp \
	$$PWD/engine/physics/simplePhysicsEngine.cpp \
	$$PWD/engine/physics/realisticPhysicsEngine.cpp \

FORMS += \
	$$PWD/engine/d2Form.ui \
