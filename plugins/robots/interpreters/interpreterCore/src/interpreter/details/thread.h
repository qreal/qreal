#pragma once

#include <QtCore/QObject>
#include <QtCore/QStack>
#include <QtCore/QSignalMapper>

#include <qrkernel/ids.h>
#include <qrgui/mainwindow/mainWindowInterpretersInterface.h>
#include <qrgui/toolPluginInterface/usedInterfaces/graphicalModelAssistInterface.h>

#include <interpreterBase/baseBlocks/blockInterface.h>

#include "blocksTable.h"

namespace interpreterCore {
namespace interpreter {
namespace details {

/// An invocation thread simulation. Has its own stack
class Thread : public QObject
{
	Q_OBJECT

public:
	/// Creates new instance of invocation thread starting runtime from
	/// specified block
	Thread(qReal::GraphicalModelAssistInterface const *graphicalModelApi
			, qReal::gui::MainWindowInterpretersInterface &interpretersInterface
			, BlocksTable &blocksTable, qReal::Id const &initialNode);

	/// Creates new instance of invocation thread starting runtime from
	/// initial node of specified diagram
	Thread(qReal::GraphicalModelAssistInterface const *graphicalModelApi
			, qReal::gui::MainWindowInterpretersInterface &interpretersInterface
			, qReal::Id const &diagramToInterpret, BlocksTable &blocksTable);

	~Thread();

	void interpret();

signals:
	void stopped();
	void newThread(qReal::Id const &startBlockId);

private slots:
	void nextBlock(qReal::Id const &blockId);

	void stepInto(qReal::Id const &diagram);
	void finishedSteppingInto();

	void failure();

	void interpretAfterEventsProcessing(QObject *block);

private:
	void initTimer();

	qReal::Id findStartingElement(qReal::Id const &diagram) const;
	void error(QString const &message, qReal::Id const &source = qReal::Id());

	void turnOn(interpreterBase::baseBlocks::BlockInterface * const block);
	void turnOff(interpreterBase::baseBlocks::BlockInterface * const block);

	qReal::GraphicalModelAssistInterface const *mGraphicalModelApi;  // Doesn't have ownership
	qReal::gui::MainWindowInterpretersInterface &mInterpretersInterface;
	BlocksTable &mBlocksTable;
	interpreterBase::baseBlocks::BlockInterface *mCurrentBlock;  // Doesn't have ownership
	QStack<interpreterBase::baseBlocks::BlockInterface *> mStack;
	qReal::Id const mInitialDiagram;
	int mBlocksSincePreviousEventsProcessing;
	QTimer *mProcessEventsTimer;  // Has ownership
	QSignalMapper *mProcessEventsMapper;  // Has ownership
};

}
}
}
