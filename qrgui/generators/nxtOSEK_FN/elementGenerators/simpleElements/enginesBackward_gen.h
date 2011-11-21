#pragma once

#include "../simpleElementGenerator.h"

namespace qReal {
namespace generators {

class EnginesBackward_gen: public SimpleElementGenerator {
public:
	EnginesBackward_gen(NxtOSEKRobotGenerator* gen, Id elementId): SimpleElementGenerator(gen, elementId) {
	}
	
protected:
	virtual void generateBodyWithoutNextElementCall();
};

}
}
