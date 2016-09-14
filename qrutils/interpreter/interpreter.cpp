/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include "interpreter.h"

#include <QtCore/QCoreApplication>
#include <QtWidgets/QAction>
#include <QtCore/QDebug>

#include <qrtext/languageToolboxInterface.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/errorReporterInterface.h>

using namespace qReal;
using namespace interpretation;

const int maxThreadsCount = 100;

Interpreter::Interpreter(const GraphicalModelAssistInterface &graphicalModelApi
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
			, mInterpretersInterface, mInitialNodeType, currentDiagramId, mBlocksTable, "main");

	emit started();
	addThread(initialThread, "main");
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

	mThreads.remove(thread->id());
	delete thread;

	if (mThreads.isEmpty()) {
		stopInterpretation();
	}
}

void Interpreter::newThread(const Id &startBlockId, const QString &threadId)
{
	if (mThreads.contains(threadId)) {
		reportError(tr("Cannot create new thread with already occupied id %1").arg(threadId));
		stopInterpretation();
	}

	Thread * const thread = new Thread(&mGraphicalModelApi, mInterpretersInterface
			, mInitialNodeType, mBlocksTable, startBlockId, threadId);
	addThread(thread, threadId);
}

void Interpreter::addThread(Thread * const thread, const QString &threadId)
{
	if (mThreads.count() >= maxThreadsCount) {
		reportError(tr("Threads limit exceeded. Maximum threads count is %1").arg(maxThreadsCount));
		stopInterpretation();
	}

	mThreads[threadId] = thread;
	connect(thread, SIGNAL(stopped()), this, SLOT(threadStopped()));

	connect(thread, &Thread::newThread, this, &Interpreter::newThread);
	connect(thread, &Thread::killThread, this, &Interpreter::killThread);
	connect(thread, &Thread::sendMessage, this, &Interpreter::sendMessage);

	QCoreApplication::processEvents();
	if (mState != idle) {
		thread->interpret();
	}
}

void Interpreter::killThread(const QString &threadId)
{
	if (mThreads.contains(threadId)) {
		mThreads[threadId]->stop();
	} else {
		reportError(tr("Killing non-existent thread %1").arg(threadId));
	}
}

void Interpreter::sendMessage(const QString &threadId, const QString &message)
{
	if (mThreads.contains(threadId)) {
		mThreads[threadId]->newMessage(message);
	}
}

void Interpreter::reportError(const QString &message)
{
	mInterpretersInterface.errorReporter()->addError(message);
}
