#pragma once

#include "../simpleElementGenerator.h"

namespace robots {
namespace generator {

class WaitForTouchSensor_gen: public SimpleElementGenerator {
public:
	WaitForTouchSensor_gen(FuncOrientedGenerator* gen, qReal::Id elementId): SimpleElementGenerator(gen, elementId) {
	}
	
protected:
	virtual void generateBodyWithoutNextElementCall();
};

}
}
