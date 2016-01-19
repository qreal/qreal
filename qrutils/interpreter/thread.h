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

#pragma once

#include <QtCore/QObject>
#include <QtCore/QStack>
#include <QtCore/QQueue>
#include <QtCore/QSignalMapper>

#include <qrkernel/ids.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/mainWindowInterpretersInterface.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/graphicalModelAssistInterface.h>

#include <qrutils/interpreter/blockInterface.h>
#include <qrutils/interpreter/blocksTableInterface.h>
#include <qrutils/interpreter/stopReason.h>
#include <qrutils/utilsDeclSpec.h>

class QTimer;

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
	Thread(const qReal::GraphicalModelAssistInterface *graphicalModelApi
			, qReal::gui::MainWindowInterpretersInterface &interpretersInterface
			, const Id &initialNodeType
			, BlocksTableInterface &blocksTable
			, const Id &initialNode
			, const QString &threadId);

	/// Creates new instance of thread starting from initial node of specified diagram.
	/// @param graphicalModelApi - graphical model, contains diagram.
	/// @param interpretersInterface - reference to QReal main window.
	/// @param initialNodeType - the type of the element to start on diagram when stepping into it.
	/// @param diagramToInterpret - diagram, whose initial node shall be executed in a new thread.
	/// @param blocksTable - interpreter-wide table of blocks (map from ids to "code-behind" objects).
	Thread(const qReal::GraphicalModelAssistInterface *graphicalModelApi
			, qReal::gui::MainWindowInterpretersInterface &interpretersInterface
			, const Id &initialNodeType
			, const Id &diagramToInterpret
			, BlocksTableInterface &blocksTable
			, const QString &threadId);

	~Thread() override;

	/// Starts interpretation process starting from the block specified in one of the constructors.
	void interpret();

	/// Stops interpretation. Optionally reason may be specified.
	void stop(qReal::interpretation::StopReason reason = qReal::interpretation::StopReason::finised);

	/// Inserts a message to a message queue.
	void newMessage(const QString &message);

	/// If there are unprocessed messages, sets message parameter to the oldest and returns true.
	/// Returns false otherwise.
	bool getMessage(QString &message);

	/// Returns string id of a thread.
	QString id() const;

signals:
	/// Emitted when interpretation process was terminated (correctly or due to errors).
	void stopped(qReal::interpretation::StopReason reason);

	/// Emitted when one of the blocks interpreted by this thread requested new thread.
	void newThread(const qReal::Id &startBlockId, const QString &threadId);

	/// Emitted when one of the blocks wants to stop some thread.
	void killThread(const QString &threadId);

	/// Emitted when one of the blocks wants to send message to another thread.
	void sendMessage(const QString &threadId, const QString &message);

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

	const qReal::GraphicalModelAssistInterface *mGraphicalModelApi;  // Doesn't have ownership
	qReal::gui::MainWindowInterpretersInterface &mInterpretersInterface;
	const Id mInitialNodeType;
	BlocksTableInterface &mBlocksTable;
	BlockInterface *mCurrentBlock;  // Doesn't have ownership
	QStack<BlockInterface *> mStack;  // Doesn't have ownership
	const qReal::Id mInitialDiagram;
	int mBlocksSincePreviousEventsProcessing;
	QTimer *mProcessEventsTimer;  // Has ownership
	QSignalMapper *mProcessEventsMapper;  // Has ownership
	QString mId;
	QQueue<QString> mMessages;
};

}
}
