#include "readableControlFlowGenerator.h"
#include "rules/simpleRules/simpleUnvisitedRule.h"
#include "rules/simpleRules/simpleVisitedOneZoneRule.h"

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
		, QList<utils::DeepFirstSearcher::LinkInfo> const &links)
{
	SimpleUnvisitedRule unvisitedRule(mSemanticTree, id, links[0]);
	SimpleVisitedOneZoneRule visitedOneZoneRule(mSemanticTree, id, links[0]);

	applyFirstPossible(QList<SemanticTransformationRule *>()
			<< &unvisitedRule
			<< &visitedOneZoneRule);
}

void ReadableControlFlowGenerator::visitFinal(Id const &id
		, QList<utils::DeepFirstSearcher::LinkInfo> const &links)
{
	Q_UNUSED(id)
	Q_UNUSED(links)
}

void ReadableControlFlowGenerator::visitConditional(Id const &id
		, QList<utils::DeepFirstSearcher::LinkInfo> const &links)
{
}

void ReadableControlFlowGenerator::visitLoop(Id const &id
		, QList<utils::DeepFirstSearcher::LinkInfo> const &links)
{
}

void ReadableControlFlowGenerator::visitSwitch(Id const &id
		, QList<utils::DeepFirstSearcher::LinkInfo> const &links)
{
	Q_UNUSED(id)
	Q_UNUSED(links)
}

void ReadableControlFlowGenerator::visitFork(Id const &id
		, QList<utils::DeepFirstSearcher::LinkInfo> const &links)
{
	Q_UNUSED(id)
	Q_UNUSED(links)
}

void ReadableControlFlowGenerator::afterSearch()
{
	mSemanticTree->debugPrint();
}

bool ReadableControlFlowGenerator::applyFirstPossible(QList<SemanticTransformationRule *> const &rules)
{
	foreach (SemanticTransformationRule * const rule, rules) {
		if (rule->apply()) {
			return true;
		}
	}
	return false;
}
