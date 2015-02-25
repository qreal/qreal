#pragma once

#include <generatorBase/primaryControlFlowValidator.h>

namespace trik {
namespace qts {

/// Ensures that a diagram may be correctly generated into Qt Script code.
class TrikQtsControlFlowValidator : public generatorBase::PrimaryControlFlowValidator
{
public:
	TrikQtsControlFlowValidator(const qrRepo::RepoApi &repo
			, qReal::ErrorReporterInterface &errorReporter
			, generatorBase::GeneratorCustomizer &customizer
			, QObject *parent = 0);

	bool validate(const qReal::Id &diagramId, const QString &threadId) override;

	PrimaryControlFlowValidator *clone() override;
};

}
}
