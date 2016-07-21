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

#include "switchInitializationRule.h"

using namespace generatorBase::semantics;

SwitchInitializationRule::SwitchInitializationRule(SemanticTree *tree
		, const qReal::Id &id
		, QList<SemanticTransformationRule::LinkInfo> const &branches
		, const qrRepo::RepoApi &repo)
	: SemanticTransformationRule(tree, id)
	, mBranches(branches)
	, mRepo(repo)
{
}

bool SwitchInitializationRule::apply()
{
	SwitchNode * const switchNode = static_cast<SwitchNode *>(mTree->findNodeFor(mId));
	QSet<qReal::Id> visitedBranches;
	bool result = true;
	for (const LinkInfo &branch : mBranches) {
		const QString value = mRepo.property(branch.linkId, "Guard").toString();
		if (branch.targetVisited) {
			// We can`t return immediately cause we`ll get inconsistent semantic tree and it will cause segfaults.
			result = false;
			continue;
		}

		if (visitedBranches.contains(branch.target)) {
			NonZoneNode * const target = static_cast<NonZoneNode *>(mTree->findNodeFor(branch.target));
			switchNode->mergeBranch(value, target);
		} else {
			SemanticNode * const branchNode = mTree->produceNodeFor(branch.target);
			switchNode->addBranch(value, branchNode);
			visitedBranches << branch.target;
		}
	}

	return result;
}
