#pragma once

#include "../abstractElementGenerator.h"

namespace robots {
namespace generator {

// Class for simple elements such as Beep, WaitForColor etc
class SimpleElementGenerator: public AbstractElementGenerator {
public:
	SimpleElementGenerator(FuncOrientedGenerator* gen, qReal::Id const &elementId): AbstractElementGenerator(gen, elementId) {
	}
	
protected:
	virtual void generateMethodBody();
	virtual void generateBodyWithoutNextElementCall() = 0;
};

}
}
