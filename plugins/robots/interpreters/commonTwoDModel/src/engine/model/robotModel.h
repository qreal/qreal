#pragma once

#include <QtGui/QPainterPath>

#include <commonTwoDModel/robotModel/twoDRobotModel.h>

#include "sensorsConfiguration.h"

namespace twoDModel {
namespace model {

class Settings;
namespace physics {
class PhysicsEngineBase;
}

class WorldModel;
class Timeline;

class RobotModel : public QObject
{
	Q_OBJECT

public:
	enum ATime {
		DoInf,
		DoByLimit,
		End
	};

	enum WheelEnum {
		left
		, right
	};

	/// @param configurer - allows to configure various model parameters specific to a kit. Takes ownership.
	RobotModel(twoDModel::robotModel::TwoDRobotModel &robotModel
			, Settings const &settings, QObject *parent = 0);
	~RobotModel();

	void reinit();

	void clear();
	void stopRobot();
	void playSound(int timeInMs);

	void setNewMotor(int speed, uint degrees, interpreterBase::robotModel::PortInfo const &port, bool breakMode);

	SensorsConfiguration &configuration();
	/// Returns a reference to external robot description.
	twoDModel::robotModel::TwoDRobotModel *info();

	int readEncoder(interpreterBase::robotModel::PortInfo const &port) const;
	void resetEncoder(interpreterBase::robotModel::PortInfo const &port);

	QPointF position() const;
	void setPosition(QPointF const &newPos);

	qreal rotation() const;
	void setRotation(qreal angle);

	/// Returns false if robot item is dragged by user at the moment.
	bool onTheGround() const;

	QDomElement serialize(QDomDocument &target) const;
	void deserialize(const QDomElement &robotElement);

	void onRobotLiftedFromGround();
	void onRobotReturnedOnGround();

	void setMotorPortOnWheel(WheelEnum wheel, interpreterBase::robotModel::PortInfo const &port);

public slots:
	void resetPhysics(WorldModel const &worldModel);

	void recalculateParams();
	void nextFragment();

signals:
	void positionChanged(QPointF const &newPosition);
	void rotationChanged(qreal newRotation);

	/// Emitted with parameter 'true' when robot starts playing sound and 'false' if playing sound complete.
	void playingSoundChanged(bool playing);

private:
	struct Motor
	{
		int radius;
		int speed;
		int spoiledSpeed;
		int degrees;
		ATime activeTimeType;
		bool isUsed;
		bool breakMode;
	};

	QPointF rotationCenter() const;
	QVector2D robotDirectionVector() const;
	QPainterPath robotBoundingPath() const;

	Motor *initMotor(int radius, int speed, long unsigned int degrees
			, interpreterBase::robotModel::PortInfo const &port, bool isUsed);

	void countNewForces();
	void countBeep();

	void countMotorTurnover();

	void synchronizePositions();

	void nextStep();

	int varySpeed(int const speed) const;

	/// Simulated robot motors.
	/// Has ownership.
	QHash<interpreterBase::robotModel::PortInfo, Motor *> mMotors;
	/// Stores how many degrees the motor rotated on.
	QHash<interpreterBase::robotModel::PortInfo, qreal> mTurnoverEngines;
	/// Describes which wheel is driven by which motor.
	QHash<WheelEnum, interpreterBase::robotModel::PortInfo> mWheelsToMotorPortsMap;
	QHash<interpreterBase::robotModel::PortInfo, interpreterBase::robotModel::PortInfo> mMotorToEncoderPortMap;

	Settings const &mSettings;
	twoDModel::robotModel::TwoDRobotModel &mRobotModel;
	SensorsConfiguration mSensorsConfiguration;

	QPointF mPos;
	qreal mAngle;
	int mBeepTime;
	bool mIsOnTheGround;

	physics::PhysicsEngineBase *mPhysicsEngine;
};

}
}
