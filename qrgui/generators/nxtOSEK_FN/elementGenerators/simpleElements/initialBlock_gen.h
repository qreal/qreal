#pragma once

#include "../simpleElementGenerator.h"

namespace qReal {
namespace generators {

class InitialBlock_gen: public SimpleElementGenerator {
public:
	InitialBlock_gen(NxtOSEKRobotGenerator* gen, Id elementId): SimpleElementGenerator(gen, elementId) {
	}

protected:	
	virtual void generateBodyWithoutNextElementCall();
};

}
}
