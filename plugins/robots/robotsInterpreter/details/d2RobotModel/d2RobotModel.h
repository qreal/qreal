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
	void setNewMotor(int speed, uint degrees, int const port);
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
	struct Motor {
		int radius;
		int speed;
		int spoiledSpeed;
		int degrees;
		ATime activeTimeType;
		bool isUsed;
		qreal motorFactor;
	};

	struct Beep {
		unsigned freq;
		int time;
	};

	void findCollision(WallItem &wall);
	bool isRobotWallCollision(WallItem &wall);
	void setWall(int index, WallItem* wall){mRobotWalls[index] = wall;}
	bool isCollision(WallItem &wall, int i);
	bool isEdgeCollision(WallItem &wall, int i);
	void getRobotFromWall(WallItem& wall, int index);
	void getEdgeRobotFromWall(WallItem& wall, int index);
	void getFromWalls();
	void setEdgeWall(int index, WallItem* wall){mRobotEdgeWalls[index] = wall;}
	void updateCoord();
	QLineF interRobotLine(WallItem& wall);
	QLineF intersectRobotLine(WallItem& wall);
	QLineF interWallLine(WallItem& wall);
	QLineF tangentLine(WallItem& wall);
	QLineF nearRobotLine(WallItem& wall, QPointF p);
	bool wallContainsRobotPoints(WallItem& wall);
	QPointF normalPoint(QPointF A, QPointF B, QPointF C);
	void calculateForceMoment();

	void setSpeedFactor(qreal speedMul);
	void initPosition();
	Motor* initMotor(int radius, int speed, long unsigned int degrees, int port, bool isUsed);
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

	QVector2D va() const;
	QVector2D vb() const;
	QVector2D v() const;
	void setV(QVector2D &V);

	void setForce(QVector2D const &force);
	void setForceMoment(qreal forceMoment);

	qreal inertialMoment() const;

	qreal fullSpeed() const;
	qreal fullSpeedA() const;
	qreal fullSpeedB() const;

	D2ModelWidget *mD2ModelWidget;
	Motor *mMotorA;
	Motor *mMotorB;
	Motor *mMotorC;
	Beep mBeep;
	details::NxtDisplay *mDisplay;
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

	QVector2D mForce;//vector
	qreal mForceMoment;
	qreal mFric;
	QVector2D mV; //velocity vector
	QVector2D mVA;
	QVector2D mVB;

	qreal mMass;
	qreal mInertialMoment;

	qreal mFullSpeed;
	qreal mFullSpeedA;
	qreal mFullSpeedB;


	qreal mAngularVelocity;

	WallItem* mRobotWalls[4]; // Массив вершин, хранящих указатели на стены
	WallItem* mRobotEdgeWalls[4]; // Массив ребер, хранящих указатели на стены

	QList<QPointF> mEdP; // Массив вершин(стен), которые попали внутрь робота
	QPointF mP[4]; // Массив вершин робота
	QLineF mL[4]; // Массив ребер робота




};

}
}
}
}
}
