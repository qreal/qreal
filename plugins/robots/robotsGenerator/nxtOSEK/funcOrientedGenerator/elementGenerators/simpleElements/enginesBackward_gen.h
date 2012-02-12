#pragma once

#include "../simpleElementGenerator.h"

namespace robots {
namespace generator {

class EnginesBackward_gen: public SimpleElementGenerator {
public:
	EnginesBackward_gen(FuncOrientedGenerator* gen, qReal::Id elementId): SimpleElementGenerator(gen, elementId) {
	}
	
protected:
	virtual void generateBodyWithoutNextElementCall();
};

}
}
