#pragma once

#include <QtGui/QColor>

#include "interpreterBase/blocksBase/block.h"
#include "interpreterBase/robotModel/robotModelInterface.h"

namespace trikKitInterpreter {
namespace blocks {
namespace details {

class SpeakerBlock : public interpreterBase::blocksBase::Block
{
	Q_OBJECT

public:
	explicit SpeakerBlock(interpreterBase::robotModel::RobotModelInterface &robotModel);
	~SpeakerBlock() override;
	void run() override;

private:
	interpreterBase::robotModel::RobotModelInterface &mRobotModel;
};

}
}
}
