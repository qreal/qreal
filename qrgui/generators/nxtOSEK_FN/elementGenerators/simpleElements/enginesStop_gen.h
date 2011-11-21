#pragma once

#include "../simpleElementGenerator.h"

namespace qReal {
namespace generators {

class EnginesStop_gen: public SimpleElementGenerator {
public:
	EnginesStop_gen(NxtOSEKRobotGenerator* gen, Id elementId): SimpleElementGenerator(gen, elementId) {
	}
	
protected:
	virtual void generateBodyWithoutNextElementCall();
};

}
}
