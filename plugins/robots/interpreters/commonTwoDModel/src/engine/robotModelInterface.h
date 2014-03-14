#pragma once

#include <QtWidgets/QAction>
#include <QtXml/QDomElement>

//s#include "sensorsConfiguration.h"

namespace twoDModel {

class RobotModelInterface
{
public:
//	virtual SensorsConfiguration &configuration() = 0;
	virtual void clear() = 0;
	virtual void setRotation(double angle) = 0;
	virtual qreal rotateAngle() const = 0;
	virtual void setSpeedFactor(qreal speedMul) = 0;
	virtual QPointF robotPos() = 0;
	virtual void setRobotPos(QPointF const& newPos) = 0;
	virtual void serialize(QDomDocument &target) = 0;
	virtual void deserialize(const QDomElement &robotElement) = 0;
};

}
