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

#include "readableControlFlowGenerator.h"

#include "rules/simpleRules/simpleUnvisitedRule.h"
#include "rules/simpleRules/simpleVisitedOneZoneRule.h"
#include "rules/simpleRules/simpleMergedIfBranchesRule.h"
#include "rules/simpleRules/simpleIfInsideCycleRule.h"

#include "rules/ifRules/ifWithBothUnvisitedRule.h"
#include "rules/ifRules/ifWithOneVisitedRule.h"

#include "rules/loopRules/loopWithBothUnvisitedRule.h"
#include "rules/loopRules/loopWithIterationVisitedRule.h"
#include "rules/loopRules/loopWithNextVisitedRule.h"

#include "rules/switchRules/switchInitializationRule.h"
#include "rules/switchRules/mergedSwitchBranchesRule.h"

using namespace generatorBase;
using namespace qReal;
using namespace semantics;

ReadableControlFlowGenerator::ReadableControlFlowGenerator(const qrRepo::RepoApi &repo
		, ErrorReporterInterface &errorReporter
		, GeneratorCustomizer &customizer
		, PrimaryControlFlowValidator &validator
		, const Id &diagramId
		, QObject *parent
		, bool isThisDiagramMain)
	: ControlFlowGeneratorBase(repo, errorReporter, customizer, validator, diagramId, parent, isThisDiagramMain)
	, mTravelingForSecondTime(false)
{
}

ControlFlowGeneratorBase *ReadableControlFlowGenerator::cloneFor(const Id &diagramId, bool cloneForNewDiagram)
{
	ReadableControlFlowGenerator * const copy = new ReadableControlFlowGenerator(mRepo
			, mErrorReporter, mCustomizer, (cloneForNewDiagram ? *mValidator.clone() : mValidator)
			, diagramId, parent(), false);

	return copy;
}

void ReadableControlFlowGenerator::performGeneration()
{
	mAlreadyApplied.clear();
	mTravelingForSecondTime = false;
	mCantBeGeneratedIntoStructuredCode = false;

	for (int iteration = 0; iteration < 2; ++iteration) {
		do {
			mSomethingChangedThisIteration = false;
			ControlFlowGeneratorBase::performGeneration();

			if (mErrorsOccured) {
				mSemanticTree = nullptr;
				return;
			}
		} while (mSomethingChangedThisIteration);

		mTravelingForSecondTime = true;
	}
}

void ReadableControlFlowGenerator::beforeSearch()
{
}

void ReadableControlFlowGenerator::visitRegular(const Id &id
		, const QList<LinkInfo> &links)
{
	ControlFlowGeneratorBase::visitRegular(id, links);
	SimpleUnvisitedRule unvisitedRule(mSemanticTree, id, links[0]);
	SimpleVisitedOneZoneRule visitedOneZoneRule(mSemanticTree, id, links[0]);
	SimpleMergedIfBranchesRule ifMergedBranchesRule(mSemanticTree, id, links[0]);
	MergedSwitchBranchesRule switchMergedBranchesRule(mSemanticTree, id, links[0]);
	SimpleIfInsideCycleRule ifInsideCycle(mSemanticTree, id, links[0]);

	QList<SemanticTransformationRule *> rules;
	if (mTravelingForSecondTime) {
		rules << &visitedOneZoneRule << &ifInsideCycle;
	} else {
		rules << &unvisitedRule << &ifMergedBranchesRule << &switchMergedBranchesRule;
	}

	applyFirstPossible(id, rules, !mTravelingForSecondTime);
}

void ReadableControlFlowGenerator::visitConditional(const Id &id
		, const QList<LinkInfo> &links)
{
	Q_UNUSED(links)

	if (mTravelingForSecondTime) {
		return;
	}

	QPair<LinkInfo, LinkInfo> const branches(ifBranchesFor(id));

	IfWithBothUnvisitedRule bothUnvisitedRule(mSemanticTree, id
			, branches.first, branches.second);
	IfWithOneVisitedRule oneVisitedRule(mSemanticTree, id
			, branches.first, branches.second);

	applyFirstPossible(id, { &oneVisitedRule, &bothUnvisitedRule }, false);
}

void ReadableControlFlowGenerator::visitLoop(const Id &id
		, const QList<LinkInfo> &links)
{
	Q_UNUSED(links)

	if (mTravelingForSecondTime) {
		return;
	}

	QPair<LinkInfo, LinkInfo> const branches(loopBranchesFor(id));

	LoopWithBothUnvisitedRule bothUnvisitedRule(mSemanticTree, id
			, branches.first, branches.second);
	LoopWithIterationVisitedRule iterationVisitedRule(mSemanticTree, id
			, branches.first, branches.second);
	LoopWithNextVisitedRule nextVisitedRule(mSemanticTree, id
			, branches.first, branches.second);

	applyFirstPossible(id, { &bothUnvisitedRule, &iterationVisitedRule, &nextVisitedRule }, false);
}

void ReadableControlFlowGenerator::visitSwitch(const Id &id, const QList<LinkInfo> &links)
{
	SwitchInitializationRule buildingRule(mSemanticTree, id, links, mRepo);
	applyFirstPossible(id, { &buildingRule }, false);
}

void ReadableControlFlowGenerator::afterSearch()
{
}

bool ReadableControlFlowGenerator::cantBeGeneratedIntoStructuredCode() const
{
	return mCantBeGeneratedIntoStructuredCode;
}

bool ReadableControlFlowGenerator::applyFirstPossible(const Id &currentId
		, QList<SemanticTransformationRule *> const &rules
		, bool thereWillBeMoreRules)
{
	if (mAlreadyApplied[currentId]) {
		return true;
	}

	for (SemanticTransformationRule * const rule : rules) {
		if (rule->apply()) {
			mAlreadyApplied[currentId] = true;
			mSomethingChangedThisIteration = true;
			return true;
		}
	}

	if (!thereWillBeMoreRules) {
		mCantBeGeneratedIntoStructuredCode = true;
	}

	return thereWillBeMoreRules;
}
