#include "readableControlFlowGenerator.h"

using namespace qReal::robots::generators;

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

void ReadableControlFlowGenerator::visitRegular(Id const &id
		, QList<utils::DeepFirstSearcher::LinkInfo> const &links)
{
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
}

void ReadableControlFlowGenerator::visitFork(Id const &id
		, QList<utils::DeepFirstSearcher::LinkInfo> const &links)
{
}
