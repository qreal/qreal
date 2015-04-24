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

#include "switchBlock.h"

using namespace qReal::interpretation::blocks;

void SwitchBlock::run()
{
	const QString expressionValue = eval<QString>("Expression");
	if (mBranches.contains(expressionValue)) {
		emit done(mBranches[expressionValue]);
	} else {
		emit done(mDefaultBranch);
	}
}

bool SwitchBlock::initNextBlocks()
{
	mBranches.clear();
	mDefaultBranch = Id();

	const IdList links = mGraphicalModelApi->graphicalRepoApi().outgoingLinks(id());
	if (links.size() < 2) {
		error(tr("There must be at list TWO links outgoing from switch block"));
		return false;
	}

	for (const Id &linkId : links) {
		const Id targetBlockId = mGraphicalModelApi->graphicalRepoApi().otherEntityFromLink(linkId, id());
		if (targetBlockId.isNull() || targetBlockId == Id::rootId()) {
			error(tr("Outgoing link is not connected"));
			return false;
		}

		const QString condition = stringProperty(linkId, "Guard").toLower();
		if (condition.isEmpty()) {
			if (mDefaultBranch.isNull()) {
				mDefaultBranch = targetBlockId;
			} else {
				error(tr("There must be exactly one link without marker on it (default branch)"));
				return false;
			}
		} else {
			if (mBranches.contains(condition)) {
				error(tr("Duplicate case branch: '%1'").arg(condition));
				return false;
			}

			mBranches[condition] = targetBlockId;
		}
	}

	if (mDefaultBranch.isNull()) {
		error(tr("There must be a link without marker on it (default branch)"));
		return false;
	}

	return true;
}
