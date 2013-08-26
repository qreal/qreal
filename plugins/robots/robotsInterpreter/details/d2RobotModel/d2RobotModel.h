#pragma once

#include <QtCore/QObject>
#include <QtCore/QTimer>
#include <QtCore/qmath.h>

#include "d2ModelWidget.h"
#include "robotModelInterface.h"
#include "worldModel.h"
#include "timeline.h"
#include "../details/nxtDisplay.h"
#include "../../../../../qrutils/mathUtils/gaussNoise.h"

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
	D2RobotModel(QObject *parent = 0);
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
		qreal VK_mMotorFactor;
	};

	struct Beep {
		unsigned freq;
		int time;
	};

	void VK_checkCollision(WallItem &wall);
	bool VK_isRobotWallCollision(WallItem &wall);
	void VK_setWall(int index, WallItem* wall){VK_mRobotWalls[index] = wall;}
	bool VK_isCollision(WallItem &wall, int i);
	bool VK_isEdgeCollision(WallItem &wall, int i);
	void VK_getRobotFromWall(WallItem& wall, int index);
    void VK_getEdgeRobotFromWall(WallItem& wall, int index);
	void VK_setEdgeWall(int index, WallItem* wall){VK_mRobotEdgeWalls[index] = wall;}
    void VK_updateVelocity(qreal dt);
    void VK_updateCoord();
	QLineF VK_interRobotLine(WallItem& wall);
    QLineF VK_interWallLine(WallItem& wall);
	QLineF VK_nearRobotLine(WallItem& wall, QPointF p);
	QPointF VK_normalPoint(qreal x1, qreal y1, qreal x2, qreal y2, qreal x3, qreal y3);


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

	QVector2D VK_mForce;//vector
    qreal VK_mForceMoment;
    QVector2D VK_mV; //velocity vector
	QVector2D VK_mVA;
	QVector2D VK_mVB;

	void updateRegion()
	{
		VK_mBoundingRegion = mD2ModelWidget->robotBoundingPolygon(mPos, mAngle);
	}

	QPainterPath VK_mBoundingRegion;


    qreal VK_mMass;
    qreal VK_mSize;
	qreal VK_mMomentI;
	QVector2D VK_getVA() const;
	QVector2D VK_getVB() const;
	QVector2D VK_getV() const;

    void VK_nextStep();

    qreal VK_mFullSpeed;
	qreal VK_mFullSpeedA;
	qreal VK_mFullSpeedB;

	qreal VK_scalarProduct(QVector2D vector1, QVector2D vector2);
    qreal VK_vectorProduct(QVector2D vector1, QVector2D vector2);

	void VK_setV(QVector2D &V);

	qreal VK_getAngle()
    {
        return mAngle;
    }
    qreal VK_getMass()
    {
        return VK_mMass;
    }
    qreal VK_getSize()
    {
        return VK_mSize;
    }

  //  qreal VK_getFullSpeed()
  //  {
    //    return VK_mFullSpeed;
  //  }
    qreal VK_getInertiaMoment()
    {
        return VK_mMomentI;
    }

    qreal VK_getAngularVelocity()
    {
        return VK_mAngularVelocity;
    }
    QVector2D VK_getForce()
    {
        return VK_mForce;
    }
    qreal VK_getForceMoment()
    {
        return VK_mForceMoment;
    }
    void VK_setForce(QVector2D force)
    {
        VK_mForce = force;
    }
    void VK_setForceMoment(qreal forceMoment)
    {
        VK_mForceMoment = forceMoment;
    }

	qreal VK_getFullSpeed()
    {
        return VK_mFullSpeed;
    }
	qreal VK_getFullSpeedA()
    {
        return VK_mFullSpeedA;
    }
	qreal VK_getFullSpeedB()
    {
        return VK_mFullSpeedB;
    }


    qreal VK_mAngularVelocity;

	WallItem* VK_mRobotWalls[4]; // Массив вершин, хранящих указатели на стены
    WallItem* VK_mRobotEdgeWalls[4]; // Массив ребер, хранящих указатели на стены

	QList<QPointF> VK_mEdP; // Массив вершин(стен), которые попали внутрь робота
    QPointF VK_mP[4]; // Массив вершин робота
    QLineF VK_mL[4]; // Массив ребер робота




};

}
}
}
}
}
