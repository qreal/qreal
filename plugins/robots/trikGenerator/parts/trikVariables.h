#pragma once

#include "base/parts/variables.h"

namespace qReal {
namespace robots {
namespace generators {
namespace parts {

/// Adds corrections to default variables management
class TrikVariables : public Variables
{
public:
	explicit TrikVariables(QString const &pathToTemplates);

protected:
	virtual QMap<QString, int> intConstants() const;
	virtual QMap<QString, float> floatConstants() const;

	virtual QString intConstantDeclaration() const;
	virtual QString floatConstantDeclaration() const;
	virtual QString intVariableDeclaration() const;
	virtual QString floatVariableDeclaration() const;
};

}
}
}
}
