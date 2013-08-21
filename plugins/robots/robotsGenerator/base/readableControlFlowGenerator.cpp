#include "readableControlFlowGenerator.h"

#include "rules/simpleRules/simpleUnvisitedRule.h"
#include "rules/simpleRules/simpleVisitedOneZoneRule.h"
#include "rules/simpleRules/simpleMergedIfBranchesRule.h"
#include "rules/ifRules/ifWithBothUnvisitedRule.h"


using namespace qReal::robots::generators;
using namespace semantics;

ReadableControlFlowGenerator::ReadableControlFlowGenerator(
		LogicalModelAssistInterface const &logicalModel
		, GraphicalModelAssistInterface const &graphicalModel
		, ErrorReporterInterface &errorReporter
		, GeneratorCustomizer const &customizer
		, Id const &diagramId
		, QObject *parent)
	: ControlFlowGeneratorBase(logicalModel, graphicalModel, errorReporter, customizer, diagramId, parent)
{
}

void ReadableControlFlowGenerator::beforeSearch()
{
	mSemanticTree = new SemanticTree(customizer(), initialNode(), this);
}

void ReadableControlFlowGenerator::visitRegular(Id const &id
		, QList<LinkInfo> const &links)
{
	SimpleUnvisitedRule unvisitedRule(mSemanticTree, id, links[0]);
	SimpleVisitedOneZoneRule visitedOneZoneRule(mSemanticTree, id, links[0]);
	SimpleMergedIfBranchesRule mergedBranchesRule(mSemanticTree, id, links[0]);

	applyFirstPossible(id, QList<SemanticTransformationRule *>()
			<< &unvisitedRule
			<< &visitedOneZoneRule
			<< &mergedBranchesRule);
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

	QPair<LinkInfo, LinkInfo> const branches(ifBranchesFor(id));

	IfWithBothUnvisitedRule bothUnvisitedRule(mSemanticTree, id
			, branches.first, branches.second);

	applyFirstPossible(id, QList<SemanticTransformationRule *>()
			<< &bothUnvisitedRule);
}

void ReadableControlFlowGenerator::visitLoop(Id const &id
		, QList<LinkInfo> const &links)
{
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
	mSemanticTree->debugPrint();
}

bool ReadableControlFlowGenerator::applyFirstPossible(Id const &currentId
		, QList<SemanticTransformationRule *> const &rules)
{
	foreach (SemanticTransformationRule * const rule, rules) {
		if (rule->apply()) {
			return true;
		}
	}

	error(tr("This diagram cannot be generated into the structured code"), currentId);
	return false;
}
