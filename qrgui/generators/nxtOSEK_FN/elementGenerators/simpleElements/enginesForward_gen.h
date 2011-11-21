#pragma once

#include "../simpleElementGenerator.h"

namespace qReal {
namespace generators {

class EnginesForward_gen: public SimpleElementGenerator {
public:
	EnginesForward_gen(NxtOSEKRobotGenerator* gen, Id elementId): SimpleElementGenerator(gen, elementId) {
	}
	
protected:
	virtual void generateBodyWithoutNextElementCall();
};

}
}
