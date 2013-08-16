#include "gotoControlFlowGenerator.h"

using namespace qReal::robots::generators;

GotoControlFlowGenerator::GotoControlFlowGenerator(LogicalModelAssistInterface const &model
		, ErrorReporterInterface &errorReporter
		, GeneratorCustomizer const &customizer
		, Id const &diagramId
		, QObject *parent)
	: ControlFlowGeneratorBase(model, errorReporter, customizer, diagramId, parent)
{
}
