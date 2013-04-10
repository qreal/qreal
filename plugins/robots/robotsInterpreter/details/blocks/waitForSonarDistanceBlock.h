#pragma once

#include <QtCore/QObject>
#include <QtCore/QTimer>

#include "waitForSensorBlock.h"

namespace qReal
{
namespace interpreters
{
namespace robots
{
namespace details
{
namespace blocks
{

class WaitForSonarDistanceBlock : public WaitForSensorBlock
{
	Q_OBJECT

public:
	explicit WaitForSonarDistanceBlock(RobotModel * const robotModel);
	virtual ~WaitForSonarDistanceBlock() {}

protected slots:
	virtual void responseSlot(int reading);

protected:
	virtual robotParts::Sensor *sensor() const;
	virtual QString name() const;

};

}
}
}
}
}
