#pragma once

#include "../simpleElementGenerator.h"

namespace robots {
namespace generator {

class EnginesStop_gen: public SimpleElementGenerator {
public:
	EnginesStop_gen(FuncOrientedGenerator* gen, qReal::Id elementId): SimpleElementGenerator(gen, elementId) {
	}
	
protected:
	virtual void generateBodyWithoutNextElementCall();
};

}
}
