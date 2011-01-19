#pragma once

#include <QtCore/QObject>
#include <QtCore/QTimer>

#include "block.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace blocks {

class SimpleTestBlock : public Block
{
	Q_OBJECT

public:
	SimpleTestBlock(Id const &graphicalId
			, models::GraphicalModelAssistApi const &graphicalModelApi
			, BlocksTable &blocksTable);
	virtual void run();

private slots:
	void timeout();

private:
	QTimer mTimer;
	Block *mNextBlock;
};

}
}
}
}
}
