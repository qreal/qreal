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

#include "joinBlock.h"

#include <qrutils/interpreter/thread.h>

using namespace qReal::interpretation::blocks;

JoinBlock::JoinBlock()
	: mIncomingTokens(0)
{
}

void JoinBlock::run()
{
	const Id link = mGraphicalModelApi->graphicalRepoApi().outgoingLinks(id())[0];
	const QString survivingId = mLogicalModelApi->propertyByRoleName(mGraphicalModelApi->logicalId(link), "Guard")
			.toString();
	if (survivingId.isEmpty()) {
		error(tr("Link outgoing from join block must have surviving thread id in its 'Guard' property"));
		return;
	}

	if (mThread->id() != survivingId) {
		disconnect(SIGNAL(done(qReal::Id)), mThread);
		mThread->stop();
	}

	mIncomingTokens++;
	if (mIncomingTokens == mGraphicalModelApi->graphicalRepoApi().incomingLinks(id()).size()) {
		emit done(mNextBlockId);
	}
}
