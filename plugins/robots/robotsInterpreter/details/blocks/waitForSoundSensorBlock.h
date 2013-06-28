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

class WaitForSoundSensorBlock : public WaitForSensorBlock
{
	Q_OBJECT

public:
	explicit WaitForSoundSensorBlock(RobotModel * const robotModel);
	virtual ~WaitForSoundSensorBlock();

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
