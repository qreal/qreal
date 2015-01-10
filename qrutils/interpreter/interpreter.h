#pragma once

#include <QtCore/QObject>
#include <QtWidgets/QAction>

#include <qrgui/mainWindow/projectManager/projectManagementInterface.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/graphicalModelAssistInterface.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/logicalModelAssistInterface.h>
#include <qrutils/watchListWindow.h>
#include <qrtext/languageToolboxInterface.h>

#include "qrutils/interpreter/thread.h"
#include "qrutils/interpreter/blocksTableInterface.h"

namespace qReal {
namespace interpretation {

/// Interprets behavoiural diagram by sequential executing blocks.  and sending commands to robot model.
/// It is the main class for all interpretation subsystem.
class Interpreter : public QObject
{
	Q_OBJECT

public:
	/// Constructor,
	/// @param graphicalModelApi - reference to graphical model, main source of information about interpreted diagram.
	/// @param logicalModelApi - reference to logical model, which contains properties of blocks.
	/// @param interpretersInterface - reference to QReal main window. Used to highlight executing blocks and so on.
	/// @param blocksTable - an entity that will produce logic of the block by its id.
	/// @param parser - parser that is used to analyze and evaluate textual expressions inside properties of blocks.
	Interpreter(qReal::GraphicalModelAssistInterface const &graphicalModelApi
			, qReal::LogicalModelAssistInterface &logicalModelApi
			, qReal::gui::MainWindowInterpretersInterface &interpretersInterface
			, BlocksTableInterface &blocksTable
			, qrtext::LanguageToolboxInterface &languageToolbox
			);

	~Interpreter();

public slots:
	void startInterpretation();
	void stopInterpretation();

signals:
	/// Emitted when connection to robot is established, devices are configured and interpretation is started.
	void started();

	/// Emitted when program is finished or stopped by user.
	void stopped();

private slots:
	void threadStopped();
	void newThread(qReal::Id const &startBlockId);

private:
	enum InterpreterState {
		interpreting
		, waitingForDevicesConfiguredToLaunch
		, idle
	};

	void addThread(Thread * const thread);

	void reportError(QString const &message);

	qReal::GraphicalModelAssistInterface const &mGraphicalModelApi;
	qReal::LogicalModelAssistInterface &mLogicalModelApi;
	qReal::gui::MainWindowInterpretersInterface &mInterpretersInterface;

	InterpreterState mState;
	QList<Thread *> mThreads;  // Has ownership
	BlocksTableInterface &mBlocksTable;  // Has ownership

	/// Reference to a parser to be able to clear parser state when starting interpretation.
	qrtext::LanguageToolboxInterface &mLanguageToolbox;
};

}
}
