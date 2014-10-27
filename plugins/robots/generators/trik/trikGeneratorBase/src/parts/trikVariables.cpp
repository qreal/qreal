#include "trikVariables.h"

using namespace trik::parts;

TrikVariables::TrikVariables(QString const &pathToTemplates
		, interpreterBase::robotModel::RobotModelInterface const &robotModel
		, qrtext::LanguageToolboxInterface &luaToolbox)
	: Variables(pathToTemplates, robotModel, luaToolbox)
{
}

QStringList TrikVariables::expressions(qrRepo::RepoApi const &api) const
{
	auto collectVariables = [&api] (QString const &blockName, QString const &property) {
		QStringList expressions;
		for (qReal::Id const &block : api.elementsByType(blockName)) {
			if (api.hasProperty(block, property)) {
				expressions << api.stringProperty(block, property) + " = 0";
			}
		}

		return expressions;
	};

	return Variables::expressions(api)
			+ collectVariables("TrikLineDetectorToVariable", "Variable")
			+ collectVariables("TrikWaitForMessage", "Variable")
			;
}
