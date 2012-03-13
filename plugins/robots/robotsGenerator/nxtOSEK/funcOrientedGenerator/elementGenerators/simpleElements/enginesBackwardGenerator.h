#pragma once

#include "../simpleElementGenerator.h"

namespace robots {
namespace generator {

class EnginesBackwardGenerator: public SimpleElementGenerator {
public:
	EnginesBackwardGenerator(FuncOrientedGenerator* gen, qReal::Id const &elementId): SimpleElementGenerator(gen, elementId) {
	}
	
protected:
	virtual void generateBodyWithoutNextElementCall();
};

}
}
