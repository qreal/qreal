#pragma once

#include "../simpleElementGenerator.h"

namespace robots {
namespace generator {

class WaitForColorIntensity_gen: public SimpleElementGenerator {
public:
	WaitForColorIntensity_gen(FuncOrientedGenerator* gen, qReal::Id elementId): SimpleElementGenerator(gen, elementId) {
	}
	
protected:
	virtual void generateBodyWithoutNextElementCall();
};

}
}
