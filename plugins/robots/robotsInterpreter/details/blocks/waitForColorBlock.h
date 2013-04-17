#pragma once

#include <QtCore/QObject>
#include <QtCore/QTimer>

#include "waitForColorSensorBlockBase.h"
#include "../robotParts/robotModel.h"

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

class WaitForColorBlock : public WaitForColorSensorBlockBase
{
	Q_OBJECT

public:
	explicit WaitForColorBlock(RobotModel * const robotModel);
	virtual ~WaitForColorBlock() {}

protected slots:
	virtual void responseSlot(int reading);
};

}
}
}
}
}
