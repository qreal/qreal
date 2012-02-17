#pragma once

#include "../simpleElementGenerator.h"

namespace robots {
namespace generator {

class EnginesForward_gen: public SimpleElementGenerator {
public:
	EnginesForward_gen(FuncOrientedGenerator* gen, qReal::Id elementId): SimpleElementGenerator(gen, elementId) {
	}
	
protected:
	virtual void generateBodyWithoutNextElementCall();
};

}
}
