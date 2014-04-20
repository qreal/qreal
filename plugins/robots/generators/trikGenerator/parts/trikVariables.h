#pragma once

#include <generatorBase/parts/variables.h>

namespace trik {
namespace parts {

/// Adds corrections to default variables management
class TrikVariables : public generatorBase::parts::Variables
{
public:
	explicit TrikVariables(QString const &pathToTemplates);

protected:
	QMap<QString, int> intConstants() const override;
	QMap<QString, float> floatConstants() const override;
};

}
}
