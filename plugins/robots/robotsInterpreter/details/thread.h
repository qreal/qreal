#pragma once

#include <QtCore/QObject>

#include "../../../../qrkernel/ids.h"
#include "../../../../qrgui/mainwindow/mainWindowInterpretersInterface.h"

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
	~Thread();
	void interpret();

signals:
	void stopped();
	void newThread(details::blocks::Block * const startBlock);

private slots:
	void nextBlock(blocks::Block * const block);
	void failure();

private:
	gui::MainWindowInterpretersInterface &mInterpretersInterface;
	blocks::Block *mCurrentBlock;  // Doesn't have ownership
};

}
}
}
}
