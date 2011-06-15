#pragma once

#include <QtCore/QObject>
#include <QtCore/QTimer>
#include <QtCore/qmath.h>

#include "d2ModelWidget.h"
#include "iConfigurableModel.h"
#include "worldModel.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace d2Model {

const int timeInterval = 5;

class D2RobotModel : public QObject, public IConfigurableRobotModel {
	Q_OBJECT

public:
	D2RobotModel(QObject *parent = 0);
	~D2RobotModel();
	void startInit();
	void stopRobot();
	void setBeep(unsigned freq, unsigned time);
	void setNewMotor(int speed, long unsigned int degrees, int const port);
	virtual SensorsConfiguration &configuration();
	D2ModelWidget *createModelWidget();

	bool readTouchSensor(inputPort::InputPortEnum const port);
	int readSonarSensor(inputPort::InputPortEnum const port) const;
	int readColorSensor(inputPort::InputPortEnum const port) const;

private slots:
	void nextFragment();

private:
	struct Motor {
		int radius;
		int speed;
		int degrees;
	};

	struct Beep {
		unsigned freq;
		unsigned time;
	};

	D2ModelWidget *mD2ModelWidget;
	QTimer *mTimer;
	Motor *mMotorA;
	Motor *mMotorB;
	Motor *mMotorC;
	Beep mBeep;
	qreal mAngle;
	QPointF mPos;
	QPointF mRotatePoint;
	QHash<int, Motor*> mMotors;
	SensorsConfiguration mSensorsConfiguration;
	WorldModel mWorldModel;

	void init();
	Motor* initMotor(int radius, int speed, long unsigned int degrees, int port);
	void countNewCoord();
	void countBeep();
};

}
}
}
}
}
