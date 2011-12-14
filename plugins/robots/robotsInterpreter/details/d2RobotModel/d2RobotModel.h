#pragma once

#include <QtCore/QObject>
#include <QtCore/QTimer>
#include <QtCore/qmath.h>

#include "d2ModelWidget.h"
#include "robotModelInterface.h"
#include "worldModel.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace d2Model {

const int timeInterval = 5;

class D2RobotModel : public QObject, public RobotModelInterface {
	Q_OBJECT

public:
	D2RobotModel(QObject *parent = 0);
	~D2RobotModel();
	virtual void clear();
	void startInit();
	void stopRobot();
	void setBeep(unsigned freq, unsigned time);
	void setNewMotor(int speed, long unsigned int degrees, int const port);
	virtual SensorsConfiguration &configuration();
	D2ModelWidget *createModelWidget();

	int readEncoder(int/*inputPort::InputPortEnum*/  const port) const;
	void resetEncoder(int/*inputPort::InputPortEnum*/  const port);

	bool readTouchSensor(inputPort::InputPortEnum const port);
	int readSonarSensor(inputPort::InputPortEnum const port) const;
	int readColorSensor(inputPort::InputPortEnum const port) const;

	void showModelWidget();

	virtual void rotateOn(double angle);
	virtual double rotateAngle() const;

	enum ATime {
		DoInf,
		Do,
		End
	};


private slots:
	void nextFragment();

private:
	struct Motor {
		int radius;
		int speed;
		int degrees;
		QPair<ATime, qreal> activeTime;
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
	QHash<int, qreal> mTurnoverMotors;// stores how many degrees the motor rotated on
	SensorsConfiguration mSensorsConfiguration;
	WorldModel mWorldModel;

	void initPosition();
	Motor* initMotor(int radius, int speed, long unsigned int degrees, int port);
	void countNewCoord();
	void countBeep();
	QPair<QPoint, qreal> countPositionAndDirection(inputPort::InputPortEnum const port) const;
	QPair<QPoint, qreal> countPositionAndDirection(QPointF localPosition, qreal localDirection) const;

	void countOneMotorTime(Motor &motor);
	void countMotorTime();

	void countMotorTurnover();

	QImage printColorSensor(inputPort::InputPortEnum const port) const;
	int readColorFullSensor(QHash<unsigned long, int> countsColor) const;
	int readColorNoneSensor(QHash<unsigned long, int> countsColor, int n) const;
	int readSingleColorSensor(unsigned long color, QHash<unsigned long, int> countsColor, int n) const;
};

}
}
}
}
}
