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

using namespace qReal::interpretation::blocks;

void ForkBlock::run()
{
	foreach (const Id &blockId, mThreadStartBlocks) {
		emit newThread(blockId);
	}

	emit done(mNextBlockId);
}

bool ForkBlock::initNextBlocks()
{
	const IdList links = mGraphicalModelApi->graphicalRepoApi().outgoingLinks(id());

	foreach (const Id &linkId, links) {
		const Id targetBlockId = mGraphicalModelApi->graphicalRepoApi().otherEntityFromLink(linkId, id());
		if (targetBlockId.isNull()) {
			error(tr("Outgoing link is not connected"));
			return false;
		}

		if (mNextBlockId.isNull()) {
			mNextBlockId = targetBlockId;
		} else {
			mThreadStartBlocks.append(targetBlockId);
		}
	}

	if (mNextBlockId.isNull()) {
		error(tr("There must be an outgoing link, use \"End\" block to finish a program"));
		return false;
	}

	if (mThreadStartBlocks.isEmpty()) {
		error(tr("There must be at least two outgoing links"));
		return false;
	}

	return true;
}
