#pragma once

#include "../abstractElementGenerator.h"

namespace robots {
namespace generator {

//class for Loop element
class LoopGenerator: public AbstractElementGenerator {
public:
	LoopGenerator(FuncOrientedGenerator* gen, qReal::Id elementId): AbstractElementGenerator(gen, elementId) {
	}
	
protected:
	virtual void generateMethodBody();
};

}
}
