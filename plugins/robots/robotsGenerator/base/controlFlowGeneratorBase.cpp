#include "controlFlowGeneratorBase.h"

using namespace qReal::robots::generators;

ControlFlowGeneratorBase::ControlFlowGeneratorBase(
		LogicalModelAssistInterface const &logicalModel
		, GraphicalModelAssistInterface const &graphicalModel
		, ErrorReporterInterface &errorReporter
		, GeneratorCustomizer const &customizer
		, Id const &diagramId
		, QObject *parent)
	: QObject(parent)
	, mModel(logicalModel)
	, mErrorReporter(errorReporter)
	, mCustomizer(customizer)
	, mDiagram(diagramId)
	, mValidator(logicalModel, graphicalModel, errorReporter, customizer, diagramId)
{
}

bool ControlFlowGeneratorBase::preGenerationCheck()
{
	return mValidator.validate();
}

ControlFlow *ControlFlowGeneratorBase::generate()
{
	if (!preGenerationCheck()) {
		return NULL;
	}

	ControlFlow *result = new ControlFlow;
	return result;
}

enums::semantics::Semantics ControlFlowGeneratorBase::semanticsOf(qReal::Id const &id) const
{
	return mCustomizer.semanticsOf(id.type());
}

QPair<qReal::Id, qReal::Id> ControlFlowGeneratorBase::ifBranchesFor(qReal::Id const &id) const
{
	return mValidator.ifBranchesFor(id);
}
