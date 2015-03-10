#pragma once

#include <kitBase/blocksBase/common/deviceBlock.h>

#include "nxtKit/robotModel/parts/nxtSpeaker.h"

namespace utils {
class AbstractTimer;
}

namespace nxt {
namespace blocks {
namespace details {

class SpeakerBlock : public kitBase::blocksBase::common::DeviceBlock<robotModel::parts::NxtSpeaker>
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
