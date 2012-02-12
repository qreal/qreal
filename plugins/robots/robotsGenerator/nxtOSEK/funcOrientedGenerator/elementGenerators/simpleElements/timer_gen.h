#pragma once

#include "../simpleElementGenerator.h"

namespace robots {
namespace generator {

class Timer_gen: public SimpleElementGenerator {
public:
	Timer_gen(FuncOrientedGenerator* gen, qReal::Id elementId): SimpleElementGenerator(gen, elementId) {
	}
	
protected:
	virtual void generateBodyWithoutNextElementCall();
};

}
}
