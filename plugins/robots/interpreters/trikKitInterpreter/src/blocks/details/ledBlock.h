#pragma once

#include <QtGui/QColor>

#include "trikDisplayWidget.h"
#include "interpreterBase/blocksBase/block.h"
#include "interpreterBase/robotModel/robotModelInterface.h"

namespace trikKitInterpreter {
namespace blocks {
namespace details {

/// An interpreter`s implementation for LED block.
/// sets LED widget's color.
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
