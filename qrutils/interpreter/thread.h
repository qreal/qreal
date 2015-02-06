#pragma once

#include <QtCore/QObject>
#include <QtCore/QStack>
#include <QtCore/QSignalMapper>

#include <qrkernel/ids.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/mainWindowInterpretersInterface.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/graphicalModelAssistInterface.h>

#include <qrutils/interpreter/blockInterface.h>
#include <qrutils/interpreter/blocksTableInterface.h>
#include <qrutils/utilsDeclSpec.h>

namespace qReal {
namespace interpretation {

/// Program execution thread. Has currently executed block, and its own stack.
class QRUTILS_EXPORT Thread : public QObject
{
	Q_OBJECT

public:
	/// Creates new instance of thread starting from specified block.
	/// @param graphicalModelApi - graphical model, contains diagram.
	/// @param interpretersInterface - reference to QReal main window.
	/// @param initialNodeType - the type of the element to start on diagram when stepping into it.
	/// @param blocksTable - interpreter-wide table of blocks (map from ids to "code-behind" objects).
	/// @param initialNode - node that shall be executed first in this thread.
	Thread(qReal::GraphicalModelAssistInterface const *graphicalModelApi
			, qReal::gui::MainWindowInterpretersInterface &interpretersInterface
			, const Id &initialNodeType
			, BlocksTableInterface &blocksTable
			, const Id &initialNode);

	/// Creates new instance of thread starting from initial node of specified diagram.
	/// @param graphicalModelApi - graphical model, contains diagram.
	/// @param interpretersInterface - reference to QReal main window.
	/// @param initialNodeType - the type of the element to start on diagram when stepping into it.
	/// @param diagramToInterpret - diagram, whose initial node shall be executed in a new thread.
	/// @param blocksTable - interpreter-wide table of blocks (map from ids to "code-behind" objects).
	Thread(qReal::GraphicalModelAssistInterface const *graphicalModelApi
			, qReal::gui::MainWindowInterpretersInterface &interpretersInterface
			, const Id &initialNodeType
			, const Id &diagramToInterpret
			, BlocksTableInterface &blocksTable);

	~Thread() override;

	/// Starts interpretation process starting from the block specified in one of the constructors.
	void interpret();

signals:
	/// Emitted when interpretation process was terminated (correctly or due to errors).
	void stopped();

	/// Emitted when one of the blocks interpreted by this thread requested new thread.
	void newThread(const qReal::Id &startBlockId);

private slots:
	void nextBlock(const qReal::Id &blockId);

	void stepInto(const qReal::Id &diagram);
	void finishedSteppingInto();

	void failure();

	void interpretAfterEventsProcessing(QObject *block);

private:
	void initTimer();

	qReal::Id findStartingElement(const qReal::Id &diagram) const;
	void error(const QString &message, const qReal::Id &source = qReal::Id());

	void turnOn(BlockInterface * const block);
	void turnOff(BlockInterface * const block);

	qReal::GraphicalModelAssistInterface const *mGraphicalModelApi;  // Doesn't have ownership
	qReal::gui::MainWindowInterpretersInterface &mInterpretersInterface;
	const Id mInitialNodeType;
	BlocksTableInterface &mBlocksTable;
	BlockInterface *mCurrentBlock;  // Doesn't have ownership
	QStack<BlockInterface *> mStack;  // Doesn't have ownership
	const qReal::Id mInitialDiagram;
	int mBlocksSincePreviousEventsProcessing;
	QTimer *mProcessEventsTimer;  // Has ownership
	QSignalMapper *mProcessEventsMapper;  // Has ownership
};

}
}
