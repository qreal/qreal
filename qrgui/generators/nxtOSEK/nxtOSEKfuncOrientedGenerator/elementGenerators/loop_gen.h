#pragma once

#include "../abstractElementGenerator.h"

namespace qReal {
namespace generators {
namespace nxtOSEKgenerator {

//class for Loop element
class Loop_gen: public AbstractElementGenerator {
public:
	Loop_gen(NxtOSEKfuncOrientedGenerator* gen, Id elementId): AbstractElementGenerator(gen, elementId) {
	}
	
protected:
	virtual void generateMethodBody();
};

}
}
}
