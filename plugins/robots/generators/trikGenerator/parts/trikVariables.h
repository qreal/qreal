#pragma once

#include <generatorBase/parts/variables.h>

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
};

}
}
}
}
