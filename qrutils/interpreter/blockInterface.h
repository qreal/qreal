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

#include <qrkernel/ids.h>

#include "qrutils/utilsDeclSpec.h"

namespace qReal {
namespace interpretation {

class Thread;

/// An interface of the block`s implementation used in the interprer for executing it.
class QRUTILS_EXPORT BlockInterface : public QObject
{
	Q_OBJECT

public:
	~BlockInterface() override {}

	/// Starts block execution inside a given thread.
	virtual void interpret(Thread *thread) = 0;

	/// Called each time when the interpretation process fails for some reason.
	/// Block must deinitialize its recources here.
	virtual void setFailedStatus() = 0;

	/// Returns the diagram`s id of the block whitch is impemented by this object.
	virtual const qReal::Id id() const = 0;

	/// Called each time when control flow has reached the end block of the requested for stepping into diagram.
	virtual void finishedSteppingInto() = 0;

signals:
	/// Emitted each time when the block has finished its work.
	/// @param nextBlock - an id of the block that was decided to follow next.
	void done(const qReal::Id &nextBlock);

	/// Emitted each time when the block has requested new thread. Useful for fork blocks.
	/// @param startBlock - an id of the block that was decided to start the thread execution.
	void newThread(const qReal::Id &startBlock, const QString &threadId);

	/// Emitted when the block requests stopping execution of some thread.
	void killThread(const QString &threadId);

	/// Emitted when the block wants to send a message to another thread.
	void sendMessage(const QString &threadId, const QString &message);

	/// Emitted each time when the block stops its execution due to some error.
	void failure();

	/// Emitted each time when execution must be continued from the initial block of the specified diagram.
	void stepInto(const qReal::Id &diagram);
};

}
}
