#pragma once

#include <generatorBase/masterGeneratorBase.h>

namespace trik {

/// Master generator implementation for TRIK platform
class TrikQtsMasterGenerator : public generatorBase::MasterGeneratorBase
{
public:
	TrikQtsMasterGenerator(qrRepo::RepoApi const &repo
			, qReal::ErrorReporterInterface &errorReporter
			, interpreterBase::robotModel::RobotModelManagerInterface const &robotModelManager
			, qrtext::LanguageToolboxInterface &textLanguage
			, qReal::Id const &diagramId);

protected:
	generatorBase::GeneratorCustomizer *createCustomizer() override;
	QString targetPath() override;
	bool supportsGotoGeneration() const override;
};

}
