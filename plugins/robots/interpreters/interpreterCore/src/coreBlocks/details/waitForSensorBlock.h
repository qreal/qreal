#pragma once

#include <interpreterBase/blocksBase/common/waitBlock.h>

namespace interpreterCore {
namespace coreBlocks {
namespace details {

/// @todo Move abstract blocks to interpreterBase::blocksBase
class WaitForSensorBlock : public interpreterBase::blocksBase::common::WaitBlock
{
	Q_OBJECT

public:
	explicit WaitForSensorBlock(interpreterBase::robotModel::RobotModelInterface &robotModel);

	~WaitForSensorBlock() override;

	void run() override;
	void stopActiveTimerInBlock() override;

protected slots:
	virtual void responseSlot(int reading) = 0;
	void timerTimeout() override;

protected:
	void stop() override;

	interpreterBase::robotModel::PortInfo mPort;
};

}
}
}
