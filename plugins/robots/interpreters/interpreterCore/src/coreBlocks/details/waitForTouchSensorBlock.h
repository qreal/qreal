#pragma once

#include <QtCore/QObject>
#include <QtCore/QTimer>

#include "waitForSensorBlock.h"

namespace interpreterCore {
namespace coreBlocks {
namespace details {

class WaitForTouchSensorBlock : public WaitForSensorBlock
{
	Q_OBJECT

public:
	explicit WaitForTouchSensorBlock(interpreterBase::robotModel::RobotModelInterface * const robotModel);
	~WaitForTouchSensorBlock() override;

protected slots:
	virtual void responseSlot(int reading);

protected:
//	virtual robotParts::Sensor *sensor() const;
	virtual QString name() const;
};

}
}
}
