#pragma once

#include "../simpleElementGenerator.h"

namespace robots {
namespace generator {

class EnginesStopGenerator: public SimpleElementGenerator {
public:
	EnginesStopGenerator(FuncOrientedGenerator* gen, qReal::Id elementId): SimpleElementGenerator(gen, elementId) {
	}
	
protected:
	virtual void generateBodyWithoutNextElementCall();
};

}
}
