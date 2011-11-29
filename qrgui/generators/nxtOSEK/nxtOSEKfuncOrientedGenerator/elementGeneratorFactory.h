#pragma once

#include "../nxtOSEKfuncOrientedGenerator.h"
#include "abstractElementGenerator.h"

namespace qReal {
namespace generators {
namespace nxtOSEKgenerator {

class ElementGeneratorFactory {
public:
	static AbstractElementGenerator* generator(NxtOSEKfuncOrientedGenerator* gen, Id elementId);
};

}
}
}
