#pragma once

#include <QtCore/QObject>
#include <QtCore/QTimer>
#include <QtCore/qmath.h>

#include <qrutils/mathUtils/gaussNoise.h>
#include "d2ModelWidget.h"
#include "robotModelInterface.h"
#include "worldModel.h"
#include "timeline.h"
//#include "details/nxtDisplay.h"

namespace twoDModel {

namespace physics {
class PhysicsEngineBase;
}

class D2RobotModel : public QObject, public RobotModelInterface
{
	Q_OBJECT

public:
	explicit D2RobotModel(QObject *parent = 0);
	~D2RobotModel();
	virtual void clear();
	void startInit();
	void startInterpretation();
	void stopRobot();
	void setBeep(unsigned freq, unsigned time);
	void setNewMotor(int speed, uint degrees, int port, bool breakMode);
	virtual SensorsConfiguration &configuration();
	D2ModelWidget *createModelWidget();
	int readEncoder(int const port) const;
	void resetEncoder(int const port);

//	details::NxtDisplay *display()

//	int readTouchSensor(robots::enums::inputPort::InputPortEnum const port);
//	int readSonarSensor(robots::enums::inputPort::InputPortEnum const port) const;
//	int readColorSensor(robots::enums::inputPort::InputPortEnum const port) const;
//	int readLightSensor(robots::enums::inputPort::InputPortEnum const port) const;

	void showModelWidget();

	virtual void setRotation(qreal angle);
	virtual qreal rotateAngle() const;

	void setRobotPos(QPointF const &newPos);
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
	struct Engine
	{
		int radius;
		int speed;
		int spoiledSpeed;
		int degrees;
		ATime activeTimeType;
		bool isUsed;
		bool breakMode;
	};

	struct Beep
	{
		unsigned freq;
		int time;
	};

	QPointF rotationCenter() const;
	QVector2D robotDirectionVector() const;

	void setSpeedFactor(qreal speedMul);
	void initPosition();
	Engine *initEngine(int radius, int speed, long unsigned int degrees, int port, bool isUsed);
	void countNewForces();
	void countBeep();

//	QPair<QPointF, qreal> countPositionAndDirection(
//			robots::enums::inputPort::InputPortEnum const port
//			) const;

	void countMotorTurnover();

//	QImage printColorSensor(robots::enums::inputPort::InputPortEnum const port) const;
	int readColorFullSensor(QHash<uint, int> const &countsColor) const;
	int readColorNoneSensor(QHash<uint, int> const &countsColor, int n) const;
	int readSingleColorSensor(uint color, QHash<uint, int> const &countsColor, int n) const;

	void synchronizePositions();
	uint spoilColor(uint const color) const;
	uint spoilLight(uint const color) const;
	int varySpeed(int const speed) const;
	int spoilSonarReading(int const distance) const;
	int truncateToInterval(int const a, int const b, int const res) const;

	void nextStep();

	D2ModelWidget *mD2ModelWidget;
	Engine *mEngineA;
	Engine *mEngineB;
	Engine *mEngineC;
	Beep mBeep;
//	details::NxtDisplay *mDisplay;
	QPointF mRotatePoint;
	QHash<int, Engine*> mEngines;  /// @todo Arrays are not enough here?
	QHash<int, qreal> mTurnoverEngines;  // stores how many degrees the motor rotated on
	SensorsConfiguration mSensorsConfiguration;
	WorldModel mWorldModel;
	physics::PhysicsEngineBase *mPhysicsEngine;
	Timeline *mTimeline;
	qreal mSpeedFactor;
	mathUtils::GaussNoise mNoiseGen;
	bool mNeedSync;
	bool mIsRealisticPhysics;
	bool mNeedSensorNoise;
	bool mNeedMotorNoise;

	QPointF mPos;
	qreal mAngle;
};

}
