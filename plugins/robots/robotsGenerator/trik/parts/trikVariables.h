#pragma once

#include "base/parts/variables.h"

namespace qReal {
namespace robots {
namespace generators {
namespace parts {

/// Adds corrections to default variables management
class TrikVariables : public Variables
{
protected:
	virtual QMap<QString, int> intConstants() const;
	virtual QMap<QString, float> floatConstants() const;
};

}
}
}
}
