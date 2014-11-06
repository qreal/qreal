#pragma once

#include <trikGeneratorBase/trikMasterGeneratorBase.h>

namespace trik {
namespace fSharp {

/// Master generator implementation for TRIK platform
class TrikFSharpMasterGenerator : public TrikMasterGeneratorBase
{
public:
	TrikFSharpMasterGenerator(qrRepo::RepoApi const &repo
			, qReal::ErrorReporterInterface &errorReporter
			, interpreterBase::robotModel::RobotModelManagerInterface const &robotModelManager
			, qrtext::LanguageToolboxInterface &textLanguage
			, qReal::Id const &diagramId
			, QString const &generatorName);

protected:
	QString targetPath() override;
	bool supportsGotoGeneration() const override;
	void outputCode(const QString &path, const QString &code);
};

}
}
