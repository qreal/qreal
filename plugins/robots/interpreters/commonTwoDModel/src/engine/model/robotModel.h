#pragma once

#include <QtGui/QPainterPath>

#include "commonTwoDModel/robotModel/twoDRobotModel.h"
#include "sensorsConfiguration.h"

namespace twoDModel {

namespace items {
class StartPosition;
}

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

	RobotModel(twoDModel::robotModel::TwoDRobotModel &robotModel
			, const Settings &settings, QObject *parent = 0);

	~RobotModel();

	void reinit();

	void clear();
	void stopRobot();
	void playSound(int timeInMs);

	void setNewMotor(int speed, uint degrees, const interpreterBase::robotModel::PortInfo &port, bool breakMode);

	SensorsConfiguration &configuration();

	/// Returns a reference to external robot description.
	robotModel::TwoDRobotModel &info();

	int readEncoder(const interpreterBase::robotModel::PortInfo &port) const;
	void resetEncoder(const interpreterBase::robotModel::PortInfo &port);

	QPointF position() const;
	void setPosition(const QPointF &newPos);

	qreal rotation() const;
	void setRotation(qreal angle);

	/// Returns false if robot item is dragged by user at the moment.
	bool onTheGround() const;

	QDomElement serialize(QDomDocument &target) const;
	void deserialize(const QDomElement &robotElement);

	void onRobotLiftedFromGround();
	void onRobotReturnedOnGround();

	void setMotorPortOnWheel(WheelEnum wheel, const interpreterBase::robotModel::PortInfo &port);

	QRectF sensorRect(const interpreterBase::robotModel::PortInfo &port, const QPointF sensorPos) const;

	/// Returns the color of the trace that robot should draw. Transparent color may also be returned
	/// (then it is highly recommended not to draw trace at all in preformance thoughts).
	QColor markerColor() const;

	/// Moves the marker of the 2D model robot down to the floor.
	/// The robot will draw its trace on the floor after that.
	/// If the marker of another color is already drawing at the moment it will be replaced.
	void markerDown(const QColor &color);

	/// Lifts the marker of the 2D model robot up.
	/// The robot stops drawing its trace on the floor after that.
	void markerUp();

	/// Returns the item whose scene position will determine robot`s start position.
	/// Transfers ownership.
	items::StartPosition *startPositionMarker() const;

public slots:
	void resetPhysics(const WorldModel &worldModel, const Timeline &timeline);

	void recalculateParams();
	void nextFragment();

signals:
	void positionChanged(const QPointF &newPosition);
	void rotationChanged(qreal newRotation);

	/// Emitted when robot rided himself (moved on motors force, not dragged by cursor or smth) from one point to other.
	void robotRided(const QPointF &newPosition, const qreal newRotation);

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
			, const interpreterBase::robotModel::PortInfo &port, bool isUsed);

	void countNewForces();
	void countBeep();

	void countMotorTurnover();

	void synchronizePositions();

	void nextStep();

	int varySpeed(const int speed) const;

	/// Simulated robot motors.
	/// Has ownership.
	QHash<interpreterBase::robotModel::PortInfo, Motor *> mMotors;
	/// Stores how many degrees the motor rotated on.
	QHash<interpreterBase::robotModel::PortInfo, qreal> mTurnoverEngines;
	/// Describes which wheel is driven by which motor.
	QHash<WheelEnum, interpreterBase::robotModel::PortInfo> mWheelsToMotorPortsMap;
	QHash<interpreterBase::robotModel::PortInfo, interpreterBase::robotModel::PortInfo> mMotorToEncoderPortMap;

	const Settings &mSettings;
	twoDModel::robotModel::TwoDRobotModel &mRobotModel;
	SensorsConfiguration mSensorsConfiguration;

	QPointF mPos;
	qreal mAngle;
	int mBeepTime;
	bool mIsOnTheGround;
	QColor mMarker;

	physics::PhysicsEngineBase *mPhysicsEngine;

	items::StartPosition *mStartPositionMarker;  // Transfers ownership to QGraphicsScene
};

}
}
