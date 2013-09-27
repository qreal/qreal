#pragma once

#include "../base/masterGeneratorBase.h"

namespace qReal {
namespace robots {
namespace generators {
namespace trik {

class TrikMasterGenerator : public MasterGeneratorBase
{
public:
	TrikMasterGenerator(qrRepo::RepoApi const &repo
			, ErrorReporterInterface &errorReporter
			, Id const &diagramId);


protected:
	virtual GeneratorCustomizer *createCustomizer();
	virtual QString targetPath();

	virtual void beforeGeneration();

private:
	void createProjectDir(QString const &projectDir);
	int mCurInitialNodeNumber;
	QString mProjectName;
	QString mProjectDir;
};

}
}
}
}
