#pragma once

#include <masterGeneratorBase.h>

namespace qReal {
namespace robots {
namespace generators {
namespace trik {

/// Master generator implementation for TRIK platform
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
	QString currentProgramName() const;
	void createProjectDir(QString const &projectDir);

	int mCurInitialNodeNumber;
	QString mProjectName;
	QString mProjectDir;
};

}
}
}
}
