#pragma once

#include "../simpleElementGenerator.h"

namespace robots {
namespace generator {

class EnginesForwardGenerator: public SimpleElementGenerator {
public:
	EnginesForwardGenerator(FuncOrientedGenerator* gen, qReal::Id elementId): SimpleElementGenerator(gen, elementId) {
	}
	
protected:
	virtual void generateBodyWithoutNextElementCall();
};

}
}
