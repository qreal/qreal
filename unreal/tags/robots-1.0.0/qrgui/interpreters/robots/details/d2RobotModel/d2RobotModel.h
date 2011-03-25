#pragma once
#include <QtCore/QObject>
#include <QtCore/QTimer>
#include <QtCore/qmath.h>
#include "robotDrawer.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace d2Model {

const int timeInterval = 5;

class D2RobotModel : public QObject {
	Q_OBJECT

public:
	D2RobotModel(QObject *parent = 0);
	~D2RobotModel();
	void startInit();
	void stopRobot();
	void setBeep(unsigned freq, unsigned time);
	void setNewMotor(int speed, long unsigned int degrees, int const port);

	struct Motor {
		int radius;
		int speed;
		int degrees;
	};

	struct Beep {
		unsigned freq;
		unsigned time;
	};

private:
	RobotDrawer *mDrawer;
	QTimer *mTimer;
	Motor *mMotorA;
	Motor *mMotorB;
	Motor *mMotorC;
	Beep mBeep;
	qreal mAngle;
	QPointF mPos;
	QPointF mRotatePoint;
	QHash<int, Motor*> mMotors;
	void init();
	Motor* initMotor(int radius, int speed, long unsigned int degrees, int port);
	void countNewCoord();
	void countBeep();
private slots:
	void nextFragment();
};

}
}
}
}
}
