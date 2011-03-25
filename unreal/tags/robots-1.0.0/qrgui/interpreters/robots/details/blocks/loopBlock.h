#pragma once

#include <QtCore/QObject>

#include "block.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace blocks {

class LoopBlock : public Block
{
	Q_OBJECT

public:
	LoopBlock();
	virtual void run();

private:
	Block *mIterationStartBlock;
	int mIterations;

	virtual bool initNextBlocks();
	virtual void additionalInit();
};

}
}
}
}
}
