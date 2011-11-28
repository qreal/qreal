#pragma once

#include "../simpleElementGenerator.h"

namespace qReal {
namespace generators {

class WaitForColorIntensity_gen: public SimpleElementGenerator {
public:
	WaitForColorIntensity_gen(NxtOSEKRobotGenerator* gen, Id elementId): SimpleElementGenerator(gen, elementId) {
	}
	
protected:
	virtual void generateBodyWithoutNextElementCall();
};

}
}
