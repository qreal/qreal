#pragma once

#include <parts/variables.h>

namespace qReal {
namespace robots {
namespace generators {
namespace parts {

/// Adds corrections to default variables management
class FSharpVariables : public Variables
{
public:
    explicit FSharpVariables(QString const &pathToTemplates);

protected:
	virtual QMap<QString, int> intConstants() const;
	virtual QMap<QString, float> floatConstants() const;
};

}
}
}
}
