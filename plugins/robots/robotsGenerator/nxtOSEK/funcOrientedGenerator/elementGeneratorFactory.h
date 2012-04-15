#pragma once

#include "../funcOrientedGenerator.h"
#include "abstractElementGenerator.h"

namespace robots {
namespace generator {

class ElementGeneratorFactory {
public:
	static AbstractElementGenerator* generator(FuncOrientedGenerator* gen, qReal::Id const &elementId);
};

}
}
