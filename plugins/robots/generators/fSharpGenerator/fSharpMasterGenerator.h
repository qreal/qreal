#pragma once

#include <generatorBase/masterGeneratorBase.h>

namespace fSharp {

/// Master generator implementation for TRIK platform
class FSharpMasterGenerator : public generatorBase::MasterGeneratorBase
{
public:
	FSharpMasterGenerator(qrRepo::RepoApi const &repo
			, qReal::ErrorReporterInterface &errorReporter
			, interpreterBase::robotModel::RobotModelManagerInterface const &robotModelManager
			, qReal::Id const &diagramId);

protected:
	generatorBase::GeneratorCustomizer *createCustomizer() override;
	QString targetPath() override;
	bool supportsGotoGeneration() const override;
	void outputCode(const QString &path, const QString &code);

};

}
