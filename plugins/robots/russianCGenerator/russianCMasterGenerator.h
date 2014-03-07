#pragma once

#include <masterGeneratorBase.h>

namespace qReal {
namespace robots {
namespace generators {
namespace russianC {

class RussianCMasterGenerator : public MasterGeneratorBase
{
public:
	RussianCMasterGenerator(qrRepo::RepoApi const &repo
			, ErrorReporterInterface &errorReporter
			, Id const &diagramId);

protected:
	GeneratorCustomizer *createCustomizer() override;
	QString targetPath() override;

	void afterGeneration() override;

private:
	void saveImages(QString const &projectDir);

	int mCurInitialNodeNumber;
};

}
}
}
}
