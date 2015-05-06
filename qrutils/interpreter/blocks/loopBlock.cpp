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

#include "loopBlock.h"

using namespace qReal::interpretation::blocks;

LoopBlock::LoopBlock()
	: mIterations(0)
	, mFirstRun(true)
{
}

void LoopBlock::run()
{
	--mIterations;
	if (mIterations < 0) {
		mFirstRun = true;
		emit done(mNextBlockId);
	} else {
		emit done(mIterationStartBlockId);
	}
}

bool LoopBlock::initNextBlocks()
{
	bool iterationFound = false;
	bool nextFound = false;

	const IdList links = mGraphicalModelApi->graphicalRepoApi().outgoingLinks(id());

	const QString iterationNotFoundError = tr("There must be a link with \"body\" marker on it");
	foreach (const Id &linkId, links) {
		const Id targetBlockId = mGraphicalModelApi->graphicalRepoApi().otherEntityFromLink(linkId, id());
		if (targetBlockId.isNull()) {
			error(tr("Outgoing link is not connected"));
			return false;
		}

		if (stringProperty(linkId, "Guard").toLower() == "iteration") {
			if (!iterationFound) {
				mIterationStartBlockId = targetBlockId;
				iterationFound = true;
			} else {
				error(tr("Two links marked as \"body\" found"));
				return false;
			}
		} else if (stringProperty(linkId, "Guard") == "") {
			if (!nextFound) {
				mNextBlockId = targetBlockId;
				nextFound = true;
			} else {
				error(iterationNotFoundError);
				return false;
			}
		}
	}

	if (!iterationFound) {
		error(iterationNotFoundError);
		return false;
	}

	if (!nextFound) {
		error(tr("There must be a non-marked outgoing link"));
		return false;
	}

	if (mFirstRun) {
		additionalInit();
		mFirstRun = false;
	}

	return true;
}

void LoopBlock::additionalInit()
{
	mIterations = eval<int>("Iterations");
}
