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
	virtual ~Block() {};

	void interpret();
	Id const id() const;

signals:
	void done(blocks::Block * const nextBlock);

protected:
	Block();
	Block *mNextBlock;  // Does not have ownership
	models::GraphicalModelAssistApi const *mGraphicalModelApi;  // Does not have ownership
	models::LogicalModelAssistApi const *mLogicalModelApi;  // Does not have ownership
	BlocksTable *mBlocksTable;  // Does not have ownership

private slots:
	void finishedRunning();

private:
	friend class qReal::interpreters::robots::details::BlocksFactory;
	void init(Id const &graphicalId
			, models::GraphicalModelAssistApi const &graphicalModelApi
			, models::LogicalModelAssistApi const &logicalModelApi
			, BlocksTable &blocksTable);

private:
	enum State {
		idle
		, running
	};

	State mState;
	Id mGraphicalId;

	virtual void initNextBlocks();
	virtual void run() = 0;
};

}
}
}
}
}
