#pragma once

#include <interpreterBase/blocksBase/block.h>

#include "src/robotModel/parts/trikDisplay.h"

namespace trikKitInterpreter {
namespace blocks {
namespace details {

class SetBackgroundBlock : public interpreterBase::blocksBase::Block
{
public:
	explicit SetBackgroundBlock(robotModel::parts::TrikDisplay &display);

	virtual void run();

private:
	QColor propertyToColor(QString const &property) const;

	robotModel::parts::TrikDisplay &mDisplay;
};

}
}
}
