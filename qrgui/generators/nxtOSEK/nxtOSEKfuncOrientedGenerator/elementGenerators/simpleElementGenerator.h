#pragma once

#include "../abstractElementGenerator.h"

namespace qReal {
namespace generators {
namespace nxtOSEKgenerator {

//class for simple elements such as Beep, WaitForColor etc
class SimpleElementGenerator: public AbstractElementGenerator {
public:
	SimpleElementGenerator(NxtOSEKfuncOrientedGenerator* gen, Id elementId): AbstractElementGenerator(gen, elementId) {
	}
	
protected:
	virtual void generateMethodBody();
	virtual void generateBodyWithoutNextElementCall() = 0;
};

}
}
}
