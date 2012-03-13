#pragma once

#include "../simpleElementGenerator.h"

namespace robots {
namespace generator {

class WaitForColorIntensityGenerator: public SimpleElementGenerator {
public:
	WaitForColorIntensityGenerator(FuncOrientedGenerator* gen, qReal::Id elementId): SimpleElementGenerator(gen, elementId) {
	}
	
protected:
	virtual void generateBodyWithoutNextElementCall();
};

}
}
