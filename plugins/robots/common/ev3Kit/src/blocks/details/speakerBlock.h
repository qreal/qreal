#pragma once

#include <kitBase/blocksBase/common/deviceBlock.h>

#include "ev3Kit/robotModel/parts/ev3Speaker.h"

namespace utils {
class AbstractTimer;
}

namespace ev3 {
namespace blocks {
namespace details {

class SpeakerBlock : public kitBase::blocksBase::common::DeviceBlock<robotModel::parts::Ev3Speaker>
{
	Q_OBJECT

public:
	explicit SpeakerBlock(kitBase::robotModel::RobotModelInterface &robotModel);

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
