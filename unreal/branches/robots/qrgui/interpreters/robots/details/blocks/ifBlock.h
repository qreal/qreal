#pragma once

#include <QtCore/QObject>

#include "block.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace blocks {

class IfBlock : public Block
{
	Q_OBJECT

public:
	IfBlock();
	virtual void run();

private:
	Block *mElseBlock;
	QString mCondition;

	virtual bool initNextBlocks();
};

}
}
}
}
}
