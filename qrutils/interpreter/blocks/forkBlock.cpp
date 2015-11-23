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

#include "forkBlock.h"

#include <qrutils/interpreter/thread.h>

using namespace qReal::interpretation::blocks;

void ForkBlock::run()
{
	for (const QString &threadId : mThreadStartBlocks.keys()) {
		emit newThread(mThreadStartBlocks[threadId], threadId);
	}

	emit done(mNextBlockId);
}

bool ForkBlock::initNextBlocks()
{
	const IdList links = mGraphicalModelApi->graphicalRepoApi().outgoingLinks(id());
	QStringList createdIds;

	if (links.size() < 2) {
		error(tr("There must be at least two outgoing links"));
		return false;
	}

	foreach (const Id &linkId, links) {
		const Id targetBlockId = mGraphicalModelApi->graphicalRepoApi().otherEntityFromLink(linkId, id());
		if (targetBlockId.isNull()) {
			error(tr("Outgoing link is not connected"));
			return false;
		}

		QString threadId = mLogicalModelApi->propertyByRoleName(mGraphicalModelApi->logicalId(linkId), "Guard")
				.toString();
		if (threadId.isEmpty()) {
			threadId = QUuid::createUuid().toString();
			createdIds << threadId;
		}

		if (mThreadStartBlocks.contains(threadId)) {
			error(tr("Cannot create two threads with the same id %1").arg(threadId));
			return false;
		}

		mThreadStartBlocks[threadId] = targetBlockId;
	}

	if (mThreadStartBlocks.contains(mThread->id())) {
		mNextBlockId = mThreadStartBlocks[mThread->id()];
		mThreadStartBlocks.remove(mThread->id());
	} else if (!createdIds.isEmpty()) {
		mNextBlockId = mThreadStartBlocks[createdIds[0]];
		mThreadStartBlocks.remove(createdIds[0]);
	} else {
		error(tr("There must be a link that has its 'Guard' property set to the current thread id %1")
				.arg(mThread->id()));
		return false;
	}

	return true;
}
