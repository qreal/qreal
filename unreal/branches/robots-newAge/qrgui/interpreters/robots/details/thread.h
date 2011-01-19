#pragma once

#include <QtCore/QObject>

#include "../../../kernel/ids.h"
#include "../../../mainwindow/mainWindowInterpretersInterface.h"

#include "blocksTable.h"
#include "blocks/block.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {

class Thread : public QObject
{
	Q_OBJECT

public:
	Thread(gui::MainWindowInterpretersInterface &interpretersInterface
			, BlocksTable &blocksTable, Id const &initialNode);
	void interpret();

signals:
	void stopped();

public slots:
	void nextBlock(blocks::Block * const block);

private:
	gui::MainWindowInterpretersInterface &mInterpretersInterface;
	blocks::Block *mCurrentBlock;  // Doesn't have ownership
	BlocksTable &mBlocksTable;
};

}
}
}
}
