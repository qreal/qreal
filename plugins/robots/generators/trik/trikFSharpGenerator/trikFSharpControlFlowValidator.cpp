#include "trikFSharpControlFlowValidator.h"

using namespace generatorBase;
using namespace trik::fSharp;

TrikFSharpControlFlowValidator::TrikFSharpControlFlowValidator(const qrRepo::RepoApi &repo
		, qReal::ErrorReporterInterface &errorReporter
		, GeneratorCustomizer &customizer
		, QObject *parent)
	: PrimaryControlFlowValidator(repo, errorReporter, customizer, parent)
{
}

PrimaryControlFlowValidator *TrikFSharpControlFlowValidator::clone()
{
	return new TrikFSharpControlFlowValidator(mRepo, mErrorReporter, mCustomizer, parent());
}

void TrikFSharpControlFlowValidator::visitRegular(const qReal::Id &id, const QList<LinkInfo> &links)
{
	Q_UNUSED(links)
	if (id.element() == "SendMessageThreads" || id.element() == "ReceiveMessageThreads") {
		error(tr("Block type is unsupported by F# generator"), id);
	}
}

void TrikFSharpControlFlowValidator::visitJoin(const qReal::Id &id, QList<LinkInfo> &links)
{
	Q_UNUSED(links)
	error(tr("Block type is unsupported by F# generator"), id);
}
