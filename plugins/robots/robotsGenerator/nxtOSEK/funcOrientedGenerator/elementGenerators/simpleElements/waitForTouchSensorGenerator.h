#pragma once

#include "../simpleElementGenerator.h"

namespace robots {
namespace generator {

class WaitForTouchSensorGenerator: public SimpleElementGenerator {
public:
	WaitForTouchSensorGenerator(FuncOrientedGenerator* gen, qReal::Id elementId): SimpleElementGenerator(gen, elementId) {
	}
	
protected:
	virtual void generateBodyWithoutNextElementCall();
};

}
}
