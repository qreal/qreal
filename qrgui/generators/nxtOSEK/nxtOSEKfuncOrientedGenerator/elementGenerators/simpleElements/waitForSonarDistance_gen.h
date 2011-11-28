#pragma once

#include "../simpleElementGenerator.h"

namespace qReal {
namespace generators {

class WaitForSonarDistance_gen: public SimpleElementGenerator {
public:
	WaitForSonarDistance_gen(NxtOSEKRobotGenerator* gen, Id elementId): SimpleElementGenerator(gen, elementId) {
	}
	
protected:
	virtual void generateBodyWithoutNextElementCall();
};

}
}
