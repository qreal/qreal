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

#include "ifBlock.h"

using namespace qReal::interpretation::blocks;

void IfBlock::run()
{
	const bool expressionValue = eval<bool>("Condition");
	if (!errorsOccured()) {
		emit done(expressionValue ? mNextBlockId : mElseBlockId);
	}
}

bool IfBlock::initNextBlocks()
{
	// In correct case exactly 2 of this 3 would be non-null
	Id trueBlockId;
	Id falseBlockId;
	Id nonMarkedBlockId;

	const IdList links = mGraphicalModelApi->graphicalRepoApi().outgoingLinks(id());
	if (links.size() != 2) {
		error(tr("There must be exactly TWO links outgoing from if block"));
		return false;
	}

	foreach (const Id &linkId, links) {
		const Id targetBlockId = mGraphicalModelApi->graphicalRepoApi().otherEntityFromLink(linkId, id());
		if (targetBlockId.isNull() || targetBlockId == Id::rootId()) {
			error(tr("Outgoing link is not connected"));
			return false;
		}

		const QString condition = stringProperty(linkId, "Guard").toLower();
		if (condition == "true") {
			if (trueBlockId.isNull()) {
				trueBlockId = targetBlockId;
			} else {
				error(tr("Two links marked with 'true' found"));
				return false;
			}
		} else if (condition == "false") {
			if (falseBlockId.isNull()) {
				falseBlockId = targetBlockId;
			} else {
				error(tr("Two links marked with 'false' found"));
				return false;
			}
		} else {
			if (nonMarkedBlockId.isNull()) {
				nonMarkedBlockId = targetBlockId;
			} else {
				error(tr("There must be at least one link with \"true\" or \"false\" marker on it"));
				return false;
			}
		}
	}

	// Now we have correctly placed links with correct guards. Determining who is who
	if (trueBlockId.isNull()) {
		mNextBlockId = nonMarkedBlockId;
		mElseBlockId = falseBlockId;
	} else if (falseBlockId.isNull()) {
		mNextBlockId = trueBlockId;
		mElseBlockId = nonMarkedBlockId;
	} else if (nonMarkedBlockId.isNull()) {
		mNextBlockId = trueBlockId;
		mElseBlockId = falseBlockId;
	}

	return true;
}
