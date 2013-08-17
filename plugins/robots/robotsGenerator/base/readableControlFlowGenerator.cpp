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
