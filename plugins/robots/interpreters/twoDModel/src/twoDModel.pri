QT += widgets xml

INCLUDEPATH += \
	$$PWD/../include/ \
	$$PWD/../../../utils/include/ \
	$$PWD/../../../../../ \

LIBS += -L$$PWD/../../../../../bin/ -lqrkernel -lqrutils -lutils

HEADERS += \
	$$PWD/../include/configurer.h \

HEADERS += \
	$$PWD/d2ModelWidget.h \
	$$PWD/d2ModelScene.h \
	$$PWD/d2RobotModel.h \
	$$PWD/robotItem.h \
	$$PWD/sensorItem.h \
	$$PWD/rotater.h \
	$$PWD/sonarSensorItem.h \
	$$PWD/robotModelInterface.h \
	$$PWD/sensorsConfiguration.h \
	$$PWD/worldModel.h \
	$$PWD/wallItem.h \
	$$PWD/stylusItem.h \
	$$PWD/lineItem.h \
	$$PWD/colorFieldItem.h \
	$$PWD/ellipseItem.h \
	$$PWD/timeline.h \
	$$PWD/d2ModelTimer.h \
	$$PWD/gridParameters.h \
	$$PWD/constants.h \
	$$PWD/physics/physicsEngineBase.h \
	$$PWD/physics/simplePhysicsEngine.h \
	$$PWD/physics/realisticPhysicsEngine.h \

SOURCES += \
	$$PWD/d2ModelWidget.cpp \
	$$PWD/d2ModelScene.cpp \
	$$PWD/d2RobotModel.cpp \
	$$PWD/robotItem.cpp \
	$$PWD/sensorItem.cpp \
	$$PWD/rotater.cpp \
	$$PWD/sonarSensorItem.cpp \
	$$PWD/sensorsConfiguration.cpp \
	$$PWD/worldModel.cpp \
	$$PWD/wallItem.cpp \
	$$PWD/stylusItem.cpp \
	$$PWD/lineItem.cpp \
	$$PWD/colorFieldItem.cpp \
	$$PWD/ellipseItem.cpp \
	$$PWD/timeline.cpp \
	$$PWD/d2ModelTimer.cpp \
	$$PWD/gridParameters.cpp \
	$$PWD/physics/physicsEngineBase.cpp \
	$$PWD/physics/simplePhysicsEngine.cpp \
	$$PWD/physics/realisticPhysicsEngine.cpp \

FORMS += \
	$$PWD/d2Form.ui \
