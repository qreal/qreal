#include "controlFlowGeneratorBase.h"

using namespace qReal::robots::generators;

ControlFlowGeneratorBase::ControlFlowGeneratorBase(LogicalModelAssistInterface const &model
		, ErrorReporterInterface &errorReporter
		, GeneratorCustomizer const &customizer
		, Id const &diagramId
		, QObject *parent)
	: QObject(parent)
	, mModel(model)
	, mErrorReporter(errorReporter)
	, mCustomizer(customizer)
	, mDiagram(diagramId)
	, mValidator(model, errorReporter, customizer, diagramId)
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
