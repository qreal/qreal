#include "readableControlFlowGenerator.h"

using namespace qReal::robots::generators;

ReadableControlFlowGenerator::ReadableControlFlowGenerator(LogicalModelAssistInterface const &model
		, ErrorReporterInterface &errorReporter
		, GeneratorCustomizer const &customizer
		, Id const &diagramId
		, QObject *parent)
	: ControlFlowGeneratorBase(model, errorReporter, customizer, diagramId, parent)
{
}
