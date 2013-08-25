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
	, RobotsDiagramVisitor(logicalModel, customizer)
	, mModel(logicalModel)
	, mErrorReporter(errorReporter)
	, mCustomizer(customizer)
	, mDiagram(diagramId)
	, mValidator(logicalModel, graphicalModel, errorReporter, customizer, diagramId)
{
}

ControlFlowGeneratorBase::~ControlFlowGeneratorBase()
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

	mErrorsOccured = false;
	ControlFlow *result = new ControlFlow;

	// This will start dfs on model graph with processig every block
	// in subclasses which must construct control flow in handlers
	startSearch(initialNode());

	if (mErrorsOccured) {
		delete result;
		return NULL;
	}

	return result;
}

void ControlFlowGeneratorBase::error(QString const &message, Id const &id, bool critical)
{
	mErrorsOccured = true;
	if (critical) {
		mErrorReporter.addCritical(message, id);
		terminateSearch();
	} else {
		mErrorReporter.addError(message, id);
	}
}

bool ControlFlowGeneratorBase::errorsOccured() const
{
	return mErrorsOccured;
}

enums::semantics::Semantics ControlFlowGeneratorBase::semanticsOf(qReal::Id const &id) const
{
	return mCustomizer.semanticsOf(id.type());
}

qReal::Id ControlFlowGeneratorBase::initialNode() const
{
	return mValidator.initialNode();
}

QPair<LinkInfo, LinkInfo> ControlFlowGeneratorBase::ifBranchesFor(qReal::Id const &id) const
{
	return mValidator.ifBranchesFor(id);
}

QPair<LinkInfo, LinkInfo> ControlFlowGeneratorBase::loopBranchesFor(qReal::Id const &id) const
{
	return mValidator.loopBranchesFor(id);
}

GeneratorCustomizer const &ControlFlowGeneratorBase::customizer() const
{
	return mCustomizer;
}
