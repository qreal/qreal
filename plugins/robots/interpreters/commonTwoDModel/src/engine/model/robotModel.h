#pragma once

#include <interpreterBase/robotModel/robotModelInterface.h>

#include "sensorsConfiguration.h"

namespace twoDModel {

namespace physics {
class PhysicsEngineBase;
}

class D2ModelWidget;
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
	RobotModel(interpreterBase::robotModel::RobotModelInterface &robotModel, QObject *parent = 0);

	void clear();
//	void startInit();
//	void startInterpretation();
	void stopRobot();
	void playSound(int timeInMs);

	void setNewMotor(int speed, uint degrees, interpreterBase::robotModel::PortInfo const &port, bool breakMode);

	SensorsConfiguration &configuration();
	/// Returns a reference to external robot description.
	interpreterBase::robotModel::RobotModelInterface &info();

	int readEncoder(interpreterBase::robotModel::PortInfo const &port) const;
	void resetEncoder(interpreterBase::robotModel::PortInfo const &port);

	void setRotation(qreal angle);
	qreal rotateAngle() const;

	void setRobotPos(QPointF const &newPos);
	QPointF robotPos() const;

	void serialize(QDomDocument &target) const;
	void deserialize(const QDomElement &robotElement);

	void setMotorPortOnWheel(WheelEnum wheel, interpreterBase::robotModel::PortInfo const &port);

private slots:
	void recalculateParams();
	void nextFragment();

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

	void initPosition();

	Motor *initEngine(int radius, int speed, long unsigned int degrees
			, interpreterBase::robotModel::PortInfo const &port, bool isUsed);

	void countNewForces();
	void countBeep();

	void countMotorTurnover();

	void synchronizePositions();

	void nextStep();

	int mBeepTime;
	QPointF mRotatePoint;

	/// Simulated robot motors.
	/// Has ownership.
	QHash<interpreterBase::robotModel::PortInfo, Motor *> mMotors;

	/// Stores how many degrees the motor rotated on.
	QHash<interpreterBase::robotModel::PortInfo, qreal> mTurnoverEngines;

	/// Describes which wheel is driven by which motor.
	QHash<WheelEnum, interpreterBase::robotModel::PortInfo> mWheelsToMotorPortsMap;

	SensorsConfiguration mSensorsConfiguration;
	bool mNeedSync;

	QPointF mPos;
	qreal mAngle;

	interpreterBase::robotModel::RobotModelInterface &mRobotModel;

	QHash<interpreterBase::robotModel::PortInfo, interpreterBase::robotModel::PortInfo> mMotorToEncoderPortMap;
};

}
