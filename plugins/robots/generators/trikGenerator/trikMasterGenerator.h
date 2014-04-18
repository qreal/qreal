#pragma once

#include <generatorBase/masterGeneratorBase.h>

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
	GeneratorCustomizer *createCustomizer() override;
	QString targetPath() override;
	bool supportsGotoGeneration() const override;
};

}
}
}
}
