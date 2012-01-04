#pragma once

#include "../funcOrientedGenerator.h"
#include "abstractElementGenerator.h"

namespace qReal {
namespace generators {
namespace nxtOSEK {

class ElementGeneratorFactory {
public:
	static AbstractElementGenerator* generator(FuncOrientedGenerator* gen, Id elementId);
};

}
}
}
