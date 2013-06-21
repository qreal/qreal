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

class WaitForGyroscopeSensorBlock : public WaitForSensorBlock
{
	Q_OBJECT

public:
	explicit WaitForGyroscopeSensorBlock(RobotModel * const robotModel);
	virtual ~WaitForGyroscopeSensorBlock();

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
