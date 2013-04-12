#pragma once

#include <QtCore/QObject>
#include <QtCore/QTimer>
#include <QtCore/qmath.h>

#include "d2ModelWidget.h"
#include "robotModelInterface.h"
#include "worldModel.h"
#include "timeline.h"
#include "../../../../../qrutils/mathUtils/gaussNoise.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace d2Model {

int const oneReciprocalTime = 500;
int const onePercentReciprocalSpeed = 44000;
int const multiplicator = 8;
int const touchSensorStrokeIncrement = 10;
int const maxLightSensorValur = 1023;

class D2RobotModel : public QObject, public RobotModelInterface
{
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
	int readEncoder(int const port) const;
	void resetEncoder(int const port);

	int readTouchSensor(inputPort::InputPortEnum const port);
	int readSonarSensor(inputPort::InputPortEnum const port) const;
	int readColorSensor(inputPort::InputPortEnum const port) const;
	int readLightSensor(inputPort::InputPortEnum const port) const;

	void showModelWidget();

	virtual void setRotation(qreal angle);
	virtual double rotateAngle() const;

	QPointF robotPos();

	virtual void serialize(QDomDocument &target);
	virtual void deserialize(const QDomElement &robotElement);

	Timeline *timeline() const;

	void setNoiseSettings();

	enum ATime {
		DoInf,
		DoByLimit,
		End
	};

signals:
	void d2MotorTimeout();

private slots:
	void recalculateParams();
	void nextFragment();

private:
	struct Motor {
		int radius;
		int speed;
		int degrees;
		ATime activeTimeType;
		bool isUsed;
	};

	struct Beep {
		unsigned freq;
		int time;
	};

	void setSpeedFactor(qreal speedMul);
	void initPosition();
	Motor* initMotor(int radius, int speed, long unsigned int degrees, int port, bool isUsed);
	void countNewCoord();
	void countBeep();
	QPair<QPointF, qreal> countPositionAndDirection(inputPort::InputPortEnum const port) const;
	void countMotorTurnover();

	QImage printColorSensor(inputPort::InputPortEnum const port) const;
	int readColorFullSensor(QHash<unsigned long, int> countsColor) const;
	int readColorNoneSensor(QHash<unsigned long, int> const &countsColor, int n) const;
	int readSingleColorSensor(unsigned long color, QHash<unsigned long, int> const &countsColor, int n) const;

	void synchronizePositions();
	unsigned long spoilColor(unsigned long const color) const;
	unsigned long spoilLight(unsigned long const color) const;
	int varySpeed(int const speed) const;
	int spoilSonarReading(int const distance) const;
	int truncateToInterval(int const a, int const b, int const res) const;

	D2ModelWidget *mD2ModelWidget;
	Motor *mMotorA;
	Motor *mMotorB;
	Motor *mMotorC;
	Beep mBeep;
	qreal mAngle;
	QPointF mPos;
	QPointF mRotatePoint;
	QHash<int, Motor*> mMotors;  // TODO: Arrays are not enough here?
	QHash<int, qreal> mTurnoverMotors;  // stores how many degrees the motor rotated on
	SensorsConfiguration mSensorsConfiguration;
	WorldModel mWorldModel;
	Timeline *mTimeline;
	qreal mSpeedFactor;
	mathUtils::GaussNoise mNoiseGen;
	bool mNeedSync;
	bool mNeedSensorNoise;
	bool mNeedMotorNoise;
};

}
}
}
}
}
