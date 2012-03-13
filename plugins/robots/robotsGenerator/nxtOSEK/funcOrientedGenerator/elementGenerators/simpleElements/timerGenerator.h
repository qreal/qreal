#pragma once

#include "../simpleElementGenerator.h"

namespace robots {
namespace generator {

class TimerGenerator: public SimpleElementGenerator {
public:
	TimerGenerator(FuncOrientedGenerator* gen, qReal::Id const &elementId): SimpleElementGenerator(gen, elementId) {
	}
	
protected:
	virtual void generateBodyWithoutNextElementCall();
};

}
}
