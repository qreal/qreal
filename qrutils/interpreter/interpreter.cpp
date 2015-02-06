#include "interpreter.h"

#include <QtCore/QCoreApplication>
#include <QtWidgets/QAction>
#include <QtCore/QDebug>

#include <qrtext/languageToolboxInterface.h>

using namespace qReal;
using namespace interpretation;

const int maxThreadsCount = 100;

Interpreter::Interpreter(GraphicalModelAssistInterface const &graphicalModelApi
		, LogicalModelAssistInterface &logicalModelApi
		, qReal::gui::MainWindowInterpretersInterface &interpretersInterface
		, BlocksTableInterface &blocksTable
		, qrtext::LanguageToolboxInterface &languageToolbox
		, const Id &initialNodeType)
	: mGraphicalModelApi(graphicalModelApi)
	, mLogicalModelApi(logicalModelApi)
	, mInterpretersInterface(interpretersInterface)
	, mState(idle)
	, mBlocksTable(blocksTable)
	, mLanguageToolbox(languageToolbox)
	, mInitialNodeType(initialNodeType)
{
}

Interpreter::~Interpreter()
{
	qDeleteAll(mThreads);
}

void Interpreter::startInterpretation()
{
	mInterpretersInterface.errorReporter()->clear();

	if (mState != idle) {
		mInterpretersInterface.errorReporter()->addInformation(tr("Interpreter is already running"));
		return;
	}

	mBlocksTable.clear();
	mLanguageToolbox.clear();

	mState = interpreting;

	const Id currentDiagramId = mInterpretersInterface.activeDiagram();

	qReal::interpretation::Thread * const initialThread = new qReal::interpretation::Thread(&mGraphicalModelApi
			, mInterpretersInterface, mInitialNodeType, currentDiagramId, mBlocksTable);

	emit started();
	addThread(initialThread);
}

void Interpreter::stopInterpretation()
{
	mState = idle;
	qDeleteAll(mThreads);
	mThreads.clear();
	mBlocksTable.setFailure();
	emit stopped();
}

void Interpreter::threadStopped()
{
	Thread * const thread = static_cast<Thread *>(sender());

	mThreads.removeAll(thread);
	delete thread;

	if (mThreads.isEmpty()) {
		stopInterpretation();
	}
}

void Interpreter::newThread(const Id &startBlockId)
{
	Thread * const thread = new Thread(&mGraphicalModelApi, mInterpretersInterface
			, mInitialNodeType, mBlocksTable, startBlockId);
	addThread(thread);
}

void Interpreter::addThread(Thread * const thread)
{
	if (mThreads.count() >= maxThreadsCount) {
		reportError(tr("Threads limit exceeded. Maximum threads count is %1").arg(maxThreadsCount));
		stopInterpretation();
	}

	mThreads.append(thread);
	connect(thread, SIGNAL(stopped()), this, SLOT(threadStopped()));

	connect(thread, &Thread::newThread, this, &Interpreter::newThread);

	QCoreApplication::processEvents();
	if (mState != idle) {
		thread->interpret();
	}
}

void Interpreter::reportError(const QString &message)
{
	mInterpretersInterface.errorReporter()->addError(message);
}
