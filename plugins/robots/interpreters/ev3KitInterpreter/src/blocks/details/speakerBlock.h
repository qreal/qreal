#pragma once

#include <interpreterBase/blocksBase/common/deviceBlock.h>

#include "robotModel/parts/ev3Speaker.h"

namespace utils {
class AbstractTimer;
}

namespace ev3KitInterpreter {
namespace blocks {
namespace details {

class SpeakerBlock : public interpreterBase::blocksBase::common::DeviceBlock<robotModel::parts::Ev3Speaker>
{
	Q_OBJECT

public:
	explicit SpeakerBlock(interpreterBase::robotModel::RobotModelInterface &robotModel);

	void run() override;

protected:
	virtual int duration() = 0;

private slots:
	void timeout();

private:
	utils::AbstractTimer * const mTimer;  // Takes ownership
};

}
}
}
