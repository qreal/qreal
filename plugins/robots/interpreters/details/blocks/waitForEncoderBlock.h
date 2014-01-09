#pragma once

#include <QtCore/QObject>
#include <QtCore/QTimer>

#include "waitBlock.h"
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
}
}
