#pragma once

#include <generatorBase/parts/variables.h>

namespace fSharp {
namespace parts {

/// Adds corrections to default variables management
class FSharpVariables : public generatorBase::parts::Variables
{
public:
	FSharpVariables(QString const &pathToTemplates
			, interpreterBase::robotModel::RobotModelInterface const &robotModel);

protected:
	QMap<QString, int> intConstants() const override;
	QMap<QString, float> floatConstants() const override;
};

}
}
