#pragma once

#include "../simpleElementGenerator.h"

namespace qReal {
namespace generators {
namespace nxtOSEK {

class FinalNode_gen: public SimpleElementGenerator {
public:
	FinalNode_gen(FuncOrientedGenerator* gen, Id elementId): SimpleElementGenerator(gen, elementId) {
	}
	
protected:
	virtual void generateBodyWithoutNextElementCall();
};

}
}
}
