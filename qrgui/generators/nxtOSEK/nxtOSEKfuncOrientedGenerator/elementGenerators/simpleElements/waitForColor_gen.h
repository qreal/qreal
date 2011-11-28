#pragma once

#include "../simpleElementGenerator.h"

namespace qReal {
namespace generators {

class WaitForColor_gen: public SimpleElementGenerator {
public:
	WaitForColor_gen(NxtOSEKRobotGenerator* gen, Id elementId): SimpleElementGenerator(gen, elementId) {
	}
	
protected:
	virtual void generateBodyWithoutNextElementCall();
};

}
}
