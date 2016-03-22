#pragma once

#include <generatorBase/primaryControlFlowValidator.h>

namespace trik {
namespace pascalABC {

/// Ensures that a diagram may be correctly generated into PascalABC code.
class TrikPascalABCControlFlowValidator : public generatorBase::PrimaryControlFlowValidator
{
public:
	TrikPascalABCControlFlowValidator(const qrRepo::RepoApi &repo
			, qReal::ErrorReporterInterface &errorReporter
			, generatorBase::GeneratorCustomizer &customizer
			, QObject *parent = 0);

	PrimaryControlFlowValidator *clone() override;

protected:
	void visitRegular(const qReal::Id &id, const QList<generatorBase::LinkInfo> &links) override;
	void visitJoin(const qReal::Id &id, QList<generatorBase::LinkInfo> &links) override;
};

}
}
