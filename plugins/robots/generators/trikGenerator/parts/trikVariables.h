#pragma once

#include <generatorBase/parts/variables.h>

namespace trik {
namespace parts {

/// Adds corrections to default variables management
class TrikVariables : public generatorBase::parts::Variables
{
public:
	TrikVariables(QString const &pathToTemplates
			, interpreterBase::robotModel::RobotModelInterface const &robotModel
			, qrtext::LanguageToolboxInterface &luaToolbox);

private:
	QStringList expressions(qrRepo::RepoApi const &api) const override;
};

}
}
