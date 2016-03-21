#include "trikPascalABCControlFlowValidator.h"

using namespace generatorBase;
using namespace trik::pascalABC;

TrikPascalABCControlFlowValidator::TrikPascalABCControlFlowValidator(const qrRepo::RepoApi &repo
		, qReal::ErrorReporterInterface &errorReporter
		, GeneratorCustomizer &customizer
		, QObject *parent)
	: PrimaryControlFlowValidator(repo, errorReporter, customizer, parent)
{
}

PrimaryControlFlowValidator *TrikPascalABCControlFlowValidator::clone()
{
	return new TrikPascalABCControlFlowValidator(mRepo, mErrorReporter, mCustomizer, parent());
}

void TrikPascalABCControlFlowValidator::visitRegular(const qReal::Id &id, const QList<LinkInfo> &links)
{
	Q_UNUSED(links)
	if (id.element() == "SendMessageThreads" || id.element() == "ReceiveMessageThreads"
			|| id.element() == "KillThread") {
		error(tr("Block type is unsupported by PascalABC generator"), id);
	}

	PrimaryControlFlowValidator::visitRegular(id, links);
}

void TrikPascalABCControlFlowValidator::visitJoin(const qReal::Id &id, QList<LinkInfo> &links)
{
	Q_UNUSED(links)
	error(tr("Block type is unsupported by PascalABC generator"), id);
	PrimaryControlFlowValidator::visitJoin(id, links);
}
