#pragma once

#include <QtCore/QObject>

#include "../../../../kernel/ids.h"
#include "../../../../models/graphicalModelAssistApi.h"
#include "../../../../models/logicalModelAssistApi.h"

#include "../blocksTable.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace blocks {

class Block : public QObject
{
	Q_OBJECT

public:
	Block(Id const &graphicalId, models::GraphicalModelAssistApi const &graphicalModelApi
			, BlocksTable &blocksTable);
	virtual ~Block() {};

	void interpret();
	Id const id() const;

signals:
	void done(blocks::Block * const nextBlock);

protected:
	Block *mNextBlock;

private slots:
	void finishedRunning();

private:
	enum State {
		idle
		, running
	};

	State mState;
	Id const mGraphicalId;

	virtual void run() = 0;

};

}
}
}
}
}
