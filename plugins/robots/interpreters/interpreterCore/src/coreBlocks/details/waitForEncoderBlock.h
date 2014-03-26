#pragma once

#include "waitBlock.h"

namespace interpreterCore {
namespace coreBlocks {
namespace details {

class WaitForEncoderBlock : public WaitBlock
{
	Q_OBJECT

public:
	explicit WaitForEncoderBlock(RobotModel * const robotModel);
	virtual ~WaitForEncoderBlock() {}

	virtual void run();

private slots:
	void responseSlot(int reading);
	void timerTimeout();

private:
	robotParts::EncoderSensor * mEncoderSensor;  // Doesn't have ownership
};

}
}
}
