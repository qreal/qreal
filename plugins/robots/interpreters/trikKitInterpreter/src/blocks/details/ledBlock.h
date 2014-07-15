#pragma once
#include "trikDisplayWidget.h"

#include "interpreterBase/blocksBase/block.h"
#include "interpreterBase/robotModel/robotModelInterface.h"

#include <QColor>

namespace trikKitInterpreter {
namespace blocks {
namespace details {

class LedBlock : public interpreterBase::blocksBase::Block
{
	Q_OBJECT

public:
	explicit LedBlock(interpreterBase::robotModel::RobotModelInterface &robotModel);
	~LedBlock() override;

	void run() override;

private:
	interpreterBase::robotModel::RobotModelInterface &mRobotModel;

};

}
}
}
