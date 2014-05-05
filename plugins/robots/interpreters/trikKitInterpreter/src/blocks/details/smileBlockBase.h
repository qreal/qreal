#pragma once

#include <QtGui/QImage>

#include <interpreterBase/blocksBase/block.h>

#include "src/robotModel/parts/trikDisplay.h"

namespace trikKitInterpreter {
namespace blocks {
namespace details {

class SmileBlockBase : public interpreterBase::blocksBase::Block
{
public:
	SmileBlockBase(QString const &pathToImage, robotModel::parts::TrikDisplay &display);

	virtual void run();

private:
	QImage mSmile;
	robotModel::parts::TrikDisplay &mDisplay;
};

}
}
}
