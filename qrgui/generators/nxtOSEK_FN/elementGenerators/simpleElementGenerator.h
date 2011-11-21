#pragma once

#include "../abstractElementGenerator.h"

namespace qReal {
namespace generators {

//class for simple elements such as Beep, WaitForColor etc
class SimpleElementGenerator: public AbstractElementGenerator {
public:
	SimpleElementGenerator(NxtOSEKRobotGenerator* gen, Id elementId): AbstractElementGenerator(gen, elementId) {
	}
	
protected:
	virtual void generateMethodBody();
	virtual void generateBodyWithoutNextElementCall() = 0;
};

}
}
