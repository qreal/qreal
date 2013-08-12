#pragma once

#include <QtCore/QObject>
#include <QtCore/QStack>

#include "../../../../qrkernel/ids.h"
#include "../../../../qrgui/mainwindow/mainWindowInterpretersInterface.h"

#include "blocksTable.h"
#include "blocks/block.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {

/// An invocation thread simulation. Has its own stack
class Thread : public QObject
{
	Q_OBJECT

public:
	/// Creates new instance of invocation thread starting runtime from
	/// specified block
	Thread(GraphicalModelAssistInterface const *graphicalModelApi
			, gui::MainWindowInterpretersInterface &interpretersInterface
			, BlocksTable &blocksTable, Id const &initialNode);

	/// Creates new instance of invocation thread starting runtime from
	/// initial node of specified diagram
	Thread(GraphicalModelAssistInterface const *graphicalModelApi
			, gui::MainWindowInterpretersInterface &interpretersInterface
			, Id const &diagramToInterpret, BlocksTable &blocksTable);

	~Thread();
	void interpret();

signals:
	void stopped();
	void newThread(details::blocks::Block * const startBlock);

private slots:
	void nextBlock(blocks::Block * const block);

	void stepInto(Id const &diagram);
	void finishedSteppingInto();

	void failure();

private:
	Id findStartingElement(Id const &diagram) const;
	void error(QString const &message, Id const &source = Id());

	void turnOn(blocks::Block * const block);
	void turnOff(blocks::Block * const block);

	GraphicalModelAssistInterface const *mGraphicalModelApi; // Doesn't have ownership
	gui::MainWindowInterpretersInterface &mInterpretersInterface;
	BlocksTable &mBlocksTable;
	blocks::Block *mCurrentBlock;  // Doesn't have ownership
	QStack<blocks::Block *> mStack;
	Id const mInitialDiagram;
};

}
}
}
}
