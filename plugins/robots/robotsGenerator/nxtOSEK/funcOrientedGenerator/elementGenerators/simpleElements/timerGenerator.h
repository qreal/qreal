#pragma once

#include "../simpleElementGenerator.h"

namespace robots {
namespace generator {

class TimerGenerator: public SimpleElementGenerator {
public:
	TimerGenerator(FuncOrientedGenerator* gen, qReal::Id elementId): SimpleElementGenerator(gen, elementId) {
	}
	
protected:
	virtual void generateBodyWithoutNextElementCall();
};

}
}
