#pragma once

#include "waitForSensorBlock.h"

namespace interpreterCore {
namespace coreBlocks {
namespace details {

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
