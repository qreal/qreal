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

using namespace qReal::robots::generators;
using namespace semantics;

ReadableControlFlowGenerator::ReadableControlFlowGenerator(
		qrRepo::RepoApi const &repo
		, ErrorReporterInterface &errorReporter
		, GeneratorCustomizer &customizer
		, Id const &diagramId
		, QObject *parent
		, bool isThisDiagramMain)
	: ControlFlowGeneratorBase(repo, errorReporter, customizer, diagramId, parent, isThisDiagramMain)
	, mTravelingForSecondTime(false)
{
}

ControlFlowGeneratorBase *ReadableControlFlowGenerator::cloneFor(Id const &diagramId)
{
	return new ReadableControlFlowGenerator(mRepo, mErrorReporter, mCustomizer
			, diagramId, parent(), false);
}

semantics::SemanticTree *ReadableControlFlowGenerator::generate()
{
	mAlreadyApplied.clear();
	mTravelingForSecondTime = false;

	ControlFlowGeneratorBase::generate();
	if (!mSemanticTree) {
		return NULL;
	}

	mTravelingForSecondTime = true;
	startSearch(initialNode());

	if (errorsOccured()) {
		mSemanticTree = NULL;
	}

	return mSemanticTree;
}

void ReadableControlFlowGenerator::beforeSearch()
{
}

void ReadableControlFlowGenerator::visitRegular(Id const &id
		, QList<LinkInfo> const &links)
{
	SimpleUnvisitedRule unvisitedRule(mSemanticTree, id, links[0]);
	SimpleVisitedOneZoneRule visitedOneZoneRule(mSemanticTree, id, links[0]);
	SimpleMergedIfBranchesRule mergedBranchesRule(mSemanticTree, id, links[0]);
	SimpleIfInsideCycleRule ifInsideCycle(mSemanticTree, id, links[0]);

	QList<SemanticTransformationRule *> rules;
	if (mTravelingForSecondTime) {
		rules << &visitedOneZoneRule << &ifInsideCycle;
	} else {
		rules << &unvisitedRule << &mergedBranchesRule;
	}

	applyFirstPossible(id, rules, !mTravelingForSecondTime);
}

void ReadableControlFlowGenerator::visitFinal(Id const &id
		, QList<LinkInfo> const &links)
{
	Q_UNUSED(id)
	Q_UNUSED(links)
}

void ReadableControlFlowGenerator::visitConditional(Id const &id
		, QList<LinkInfo> const &links)
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

	applyFirstPossible(id, QList<SemanticTransformationRule *>()
			<< &oneVisitedRule << &bothUnvisitedRule, false);
}

void ReadableControlFlowGenerator::visitLoop(Id const &id
		, QList<LinkInfo> const &links)
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

	applyFirstPossible(id, QList<SemanticTransformationRule *>()
			<< &bothUnvisitedRule
			<< &iterationVisitedRule
			<< &nextVisitedRule, false);
}

void ReadableControlFlowGenerator::visitSwitch(Id const &id
		, QList<LinkInfo> const &links)
{
	Q_UNUSED(id)
	Q_UNUSED(links)
}

void ReadableControlFlowGenerator::visitFork(Id const &id
		, QList<LinkInfo> const &links)
{
	Q_UNUSED(id)
	Q_UNUSED(links)
}

void ReadableControlFlowGenerator::afterSearch()
{
}

bool ReadableControlFlowGenerator::applyFirstPossible(Id const &currentId
		, QList<SemanticTransformationRule *> const &rules
		, bool thereWillBeMoreRules)
{
	if (mAlreadyApplied[currentId]) {
		return true;
	}

	foreach (SemanticTransformationRule * const rule, rules) {
		if (rule->apply()) {
			mAlreadyApplied[currentId] = true;
			return true;
		}
	}

	if (!thereWillBeMoreRules) {
		error(tr("This diagram cannot be generated into the structured code"), currentId);
	}

	return thereWillBeMoreRules;
}
