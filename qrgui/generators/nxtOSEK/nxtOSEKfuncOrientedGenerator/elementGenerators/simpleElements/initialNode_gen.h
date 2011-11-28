#pragma once

#include "../simpleElementGenerator.h"

namespace qReal {
namespace generators {

class InitialNode_gen: public SimpleElementGenerator {
public:
	InitialNode_gen(NxtOSEKRobotGenerator* gen, Id elementId): SimpleElementGenerator(gen, elementId) {
	}
	
protected:
	virtual void generateBodyWithoutNextElementCall();
};

}
}
