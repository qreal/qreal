#include "gotoControlFlowGenerator.h"

using namespace qReal::robots::generators;

GotoControlFlowGenerator::GotoControlFlowGenerator(
		qrRepo::RepoApi const &repo
		, ErrorReporterInterface &errorReporter
		, GeneratorCustomizer &customizer
		, Id const &diagramId
		, QObject *parent)
	: ControlFlowGeneratorBase(repo, errorReporter, customizer, diagramId, parent)
{
}
