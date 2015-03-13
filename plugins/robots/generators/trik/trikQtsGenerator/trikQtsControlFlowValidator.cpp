#include "trikQtsControlFlowValidator.h"

#include "threadsValidator.h"

using namespace generatorBase;
using namespace trik::qts;

TrikQtsControlFlowValidator::TrikQtsControlFlowValidator(const qrRepo::RepoApi &repo
		, qReal::ErrorReporterInterface &errorReporter
		, GeneratorCustomizer &customizer
		, QObject *parent)
	: PrimaryControlFlowValidator(repo, errorReporter, customizer, parent)
{
}

bool TrikQtsControlFlowValidator::validate(const qReal::Id &diagramId, const QString &threadId)
{
	mDiagram = diagramId;
	findInitialNode();
	if (mInitialNode.isNull()) {
		error(QObject::tr("There is nothing to generate, diagram doesn't have Initial Node"), mDiagram);
		return false;
	}

	ThreadsValidator threadsValidator(mRepo, mCustomizer, mErrorReporter);
	return threadsValidator.validate(mInitialNode, threadId)
			&& PrimaryControlFlowValidator::validate(diagramId, threadId);
}

PrimaryControlFlowValidator *TrikQtsControlFlowValidator::clone()
{
	return new TrikQtsControlFlowValidator(mRepo, mErrorReporter, mCustomizer, parent());
}
