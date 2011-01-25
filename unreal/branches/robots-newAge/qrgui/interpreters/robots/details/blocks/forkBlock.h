#pragma once

#include <QtCore/QObject>

#include "block.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace blocks {

class ForkBlock : public Block
{
	Q_OBJECT

public:
	ForkBlock();
	virtual void run();

private:
	Block *mThreadStartBlock;

	virtual void initNextBlocks();
};

}
}
}
}
}
