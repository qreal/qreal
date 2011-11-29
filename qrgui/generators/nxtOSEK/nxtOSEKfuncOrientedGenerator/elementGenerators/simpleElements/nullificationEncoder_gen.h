#pragma once

#include "../simpleElementGenerator.h"

namespace qReal {
namespace generators {
namespace nxtOSEKgenerator {

class NullificationEncoder_gen: public SimpleElementGenerator {
public:
	NullificationEncoder_gen(NxtOSEKfuncOrientedGenerator* gen, Id elementId): SimpleElementGenerator(gen, elementId) {
	}
	
protected:
	virtual void generateBodyWithoutNextElementCall();
};

}
}
}
