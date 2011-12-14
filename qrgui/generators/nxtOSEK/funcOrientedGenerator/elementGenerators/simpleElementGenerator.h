#pragma once

#include "../abstractElementGenerator.h"

namespace qReal {
namespace generators {
namespace nxtOSEK {

//class for simple elements such as Beep, WaitForColor etc
class SimpleElementGenerator: public AbstractElementGenerator {
public:
	SimpleElementGenerator(FuncOrientedGenerator* gen, Id elementId): AbstractElementGenerator(gen, elementId) {
	}
	
protected:
	virtual void generateMethodBody();
	virtual void generateBodyWithoutNextElementCall() = 0;
};

}
}
}
