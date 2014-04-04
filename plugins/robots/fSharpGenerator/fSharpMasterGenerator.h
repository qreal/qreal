#pragma once

#include <masterGeneratorBase.h>

namespace qReal {
namespace robots {
namespace generators {
namespace fSharp {

/// Master generator implementation for FSharp platform
class FSharpMasterGenerator : public MasterGeneratorBase
{
public:
	FSharpMasterGenerator(qrRepo::RepoApi const &repo
			, ErrorReporterInterface &errorReporter
			, Id const &diagramId);

protected:
	GeneratorCustomizer *createCustomizer() override;
	QString targetPath() override;
	bool supportsGotoGeneration() const override;
    void outputCode(QString const &path, QString const &code);

};

}
}
}
}
