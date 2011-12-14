#pragma once

#include "../simpleElementGenerator.h"

namespace qReal {
namespace generators {
namespace nxtOSEK {

class WaitForColorIntensity_gen: public SimpleElementGenerator {
public:
	WaitForColorIntensity_gen(FuncOrientedGenerator* gen, Id elementId): SimpleElementGenerator(gen, elementId) {
	}
	
protected:
	virtual void generateBodyWithoutNextElementCall();
};

}
}
}
