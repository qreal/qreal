#pragma once

#include "nxtOSEKRobotGenerator.h"
#include "abstractElementGenerator.h"

namespace qReal {
namespace generators {

class ElementGeneratorFactory {
public:
	static AbstractElementGenerator* generator(NxtOSEKRobotGenerator* gen, Id elementId);
};

}
}

