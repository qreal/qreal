#pragma once

#include "../simpleElementGenerator.h"

namespace qReal {
namespace generators {
namespace nxtOSEKgenerator {

class InitialBlock_gen: public SimpleElementGenerator {
public:
	InitialBlock_gen(NxtOSEKfuncOrientedGenerator* gen, Id elementId): SimpleElementGenerator(gen, elementId) {
	}

protected:	
	virtual void generateBodyWithoutNextElementCall();
};

}
}
}
