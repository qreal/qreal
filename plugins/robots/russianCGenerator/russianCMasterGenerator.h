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
	virtual GeneratorCustomizer *createCustomizer();
	virtual QString targetPath();

	virtual void beforeGeneration();
	virtual void afterGeneration();

private:
	void createProjectDir(QString const &projectDir);

	void generateOilFile(QString const &projectName, QString const &projectDir);
	void generateMakeFile(QString const &projectName, QString const &projectDir);
	void saveImages(QString const &projectDir);

	int mCurInitialNodeNumber;
};

}
}
}
}
