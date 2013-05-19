#pragma once

#include <QtCore/QObject>
#include <QtCore/QTimer>

#include "waitForColorSensorBlockBase.h"

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

class WaitForColorIntensityBlock : public WaitForColorSensorBlockBase
{
	Q_OBJECT

public:
	explicit WaitForColorIntensityBlock(RobotModel * const robotModel);
	virtual ~WaitForColorIntensityBlock() {}

protected slots:
	void responseSlot(int reading);

};

}
}
}
}
}
