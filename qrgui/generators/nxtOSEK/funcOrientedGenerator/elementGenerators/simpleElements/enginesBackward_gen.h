#pragma once

#include "../simpleElementGenerator.h"

namespace qReal {
namespace generators {
namespace nxtOSEK {

class EnginesBackward_gen: public SimpleElementGenerator {
public:
	EnginesBackward_gen(FuncOrientedGenerator* gen, Id elementId): SimpleElementGenerator(gen, elementId) {
	}
	
protected:
	virtual void generateBodyWithoutNextElementCall();
};

}
}
}
