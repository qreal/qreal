#pragma once

#include <QtCore/QObject>
#include <QtCore/QTimer>
#include <QtCore/qmath.h>

#include <qrutils/mathUtils/gaussNoise.h>
#include "d2ModelWidget.h"
#include "robotModelInterface.h"
#include "worldModel.h"
#include "timeline.h"
#include "details/nxtDisplay.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace d2Model {

qreal const onePercentAngularVelocity = 0.0055;
int const touchSensorWallStrokeIncrement = 10;
int const touchSensorStrokeIncrement = 5;
int const maxLightSensorValur = 1023;

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

	details::NxtDisplay *display();

	int readTouchSensor(robots::enums::inputPort::InputPortEnum const port);
	int readSonarSensor(robots::enums::inputPort::InputPortEnum const port) const;
	int readColorSensor(robots::enums::inputPort::InputPortEnum const port) const;
	int readLightSensor(robots::enums::inputPort::InputPortEnum const port) const;

	void showModelWidget();

	virtual void setRotation(qreal angle);
	virtual double rotateAngle() const;

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
		qreal motorFactor;
		bool breakMode;
	};

	struct Beep
	{
		unsigned freq;
		int time;
	};

	/// Counts and returns traction force vector taking into consideration engines speed and placement
	void countTractionForceAndItsMoment(qreal speed1, qreal speed2, bool breakMode);
	void recalculateVelocity();
	void applyRotationalFrictionForce();

	QVector2D robotDirectionVector() const;

	void findCollision(WallItem const &wall);

	bool isRobotWallCollision(WallItem const &wall) const;
	bool isCollision(WallItem const &wall, int i) const;
	bool isEdgeCollision(WallItem const &wall, int i) const;

	void getRobotFromWall(WallItem const &wall, int index);
	void getEdgeRobotFromWall(WallItem const &wall, int index);
	void getFromWalls();

	void setWall(int index, WallItem const *wall);
	void setEdgeWall(int index, WallItem const *wall);

	/// Recalculates robot`s vertices and borders due to current position
	void updateCoord();
	/// Returns the first wall border that intersects robot polygon
	QLineF wallBorderIntersectedByRobot(WallItem const &wall) const;
	/// Returns a closest to a given point border of the given wall
	QLineF closestWallBorder(WallItem const &wall, QPointF const &point) const;

	void setSpeedFactor(qreal speedMul);
	void initPosition();
	Engine *initEngine(int radius, int speed, long unsigned int degrees, int port, bool isUsed);
	void countNewCoord();
	void countBeep();

	QPair<QPointF, qreal> countPositionAndDirection(
			robots::enums::inputPort::InputPortEnum const port
			) const;

	void countMotorTurnover();

	QImage printColorSensor(robots::enums::inputPort::InputPortEnum const port) const;
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
	details::NxtDisplay *mDisplay;
	QPointF mRotatePoint;
	QHash<int, Engine*> mEngines;  // TODO: Arrays are not enough here?
	QHash<int, qreal> mTurnoverEngines;  // stores how many degrees the motor rotated on
	SensorsConfiguration mSensorsConfiguration;
	WorldModel mWorldModel;
	Timeline *mTimeline;
	qreal mSpeedFactor;
	mathUtils::GaussNoise mNoiseGen;
	bool mNeedSync;
	bool mNeedSensorNoise;
	bool mNeedMotorNoise;

	QPointF mPos;
	qreal mAngle;

	qreal mInertialMoment;
	QVector2D mTractionForce;
	qreal mForceMoment;
	qreal mMass;

	qreal mAngularVelocity;
	QVector2D mVelocity;

	WallItem const *mRobotWalls[4];
	WallItem const *mRobotEdgeWalls[4];

	QList<QPointF> mInsidePoints; // An array of wall`s vertices that got inside the robot
	QPointF mVertices[4]; // Robot rect`s corners
	QLineF mBorders[4]; // Robot rect`s borders
};

}
}
}
}
}
