QT += widgets xml

INCLUDEPATH += \
	$$PWD/include/ \
	$$PWD/../../utils/include/ \
	$$PWD/../../../../ \

LIBS += -L$$PWD/../../../../bin/ -lqrkernel -lqrutils -lutils -linterpreterBase

HEADERS += \
	$$PWD/include/commonTwoDModel/engine/configurer.h \
	$$PWD/include/commonTwoDModel/engine/twoDModelFacade.h \
	$$PWD/include/commonTwoDModel/engine/twoDModelControlInterface.h \

HEADERS += \
	$$PWD/src/engine/d2ModelWidget.h \
	$$PWD/src/engine/d2ModelScene.h \
	$$PWD/src/engine/robotItem.h \
	$$PWD/src/engine/sensorItem.h \
	$$PWD/src/engine/rotater.h \
	$$PWD/src/engine/sonarSensorItem.h \
	$$PWD/src/engine/sensorsConfiguration.h \
	$$PWD/src/engine/worldModel.h \
	$$PWD/src/engine/wallItem.h \
	$$PWD/src/engine/stylusItem.h \
	$$PWD/src/engine/lineItem.h \
	$$PWD/src/engine/colorFieldItem.h \
	$$PWD/src/engine/ellipseItem.h \
	$$PWD/src/engine/timeline.h \
	$$PWD/src/engine/d2ModelTimer.h \
	$$PWD/src/engine/gridParameters.h \
	$$PWD/src/engine/constants.h \
	$$PWD/src/engine/d2RobotModel.h \
	$$PWD/src/engine/robotModelInterface.h \
	$$PWD/src/engine/physics/physicsEngineBase.h \
	$$PWD/src/engine/physics/simplePhysicsEngine.h \
	$$PWD/src/engine/physics/realisticPhysicsEngine.h \

SOURCES += \
	$$PWD/src/engine/d2ModelWidget.cpp \
	$$PWD/src/engine/d2ModelScene.cpp \
	$$PWD/src/engine/d2RobotModel.cpp \
	$$PWD/src/engine/robotItem.cpp \
	$$PWD/src/engine/sensorItem.cpp \
	$$PWD/src/engine/rotater.cpp \
	$$PWD/src/engine/sonarSensorItem.cpp \
	$$PWD/src/engine/sensorsConfiguration.cpp \
	$$PWD/src/engine/worldModel.cpp \
	$$PWD/src/engine/wallItem.cpp \
	$$PWD/src/engine/stylusItem.cpp \
	$$PWD/src/engine/lineItem.cpp \
	$$PWD/src/engine/colorFieldItem.cpp \
	$$PWD/src/engine/ellipseItem.cpp \
	$$PWD/src/engine/timeline.cpp \
	$$PWD/src/engine/d2ModelTimer.cpp \
	$$PWD/src/engine/gridParameters.cpp \
	$$PWD/src/engine/twoDModelFacade.cpp \
	$$PWD/src/engine/physics/physicsEngineBase.cpp \
	$$PWD/src/engine/physics/simplePhysicsEngine.cpp \
	$$PWD/src/engine/physics/realisticPhysicsEngine.cpp \

FORMS += \
	$$PWD/src/engine/d2Form.ui \
