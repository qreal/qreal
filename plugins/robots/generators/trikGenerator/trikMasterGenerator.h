#pragma once

#include <generatorBase/masterGeneratorBase.h>

namespace trik {

/// Master generator implementation for TRIK platform
class TrikMasterGenerator : public generatorBase::MasterGeneratorBase
{
public:
	TrikMasterGenerator(qrRepo::RepoApi const &repo
			, qReal::ErrorReporterInterface &errorReporter
			, interpreterBase::robotModel::RobotModelManagerInterface const &robotModelManager
			, qReal::Id const &diagramId);

protected:
	generatorBase::GeneratorCustomizer *createCustomizer() override;
	QString targetPath() override;
	bool supportsGotoGeneration() const override;
};

}
