#pragma once

#include <generatorBase/masterGeneratorBase.h>

namespace russianC {

class RussianCMasterGenerator : public generatorBase::MasterGeneratorBase
{
public:
	RussianCMasterGenerator(qrRepo::RepoApi const &repo
			, qReal::ErrorReporterInterface &errorReporter
			, qReal::Id const &diagramId);

protected:
	generatorBase::GeneratorCustomizer *createCustomizer() override;
	QString targetPath() override;
	bool supportsGotoGeneration() const override;

	void afterGeneration() override;

private:
	void saveImages(QString const &projectDir);

	int mCurInitialNodeNumber;
};

}
