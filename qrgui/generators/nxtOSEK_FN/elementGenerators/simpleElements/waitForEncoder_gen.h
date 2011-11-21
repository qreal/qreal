#pragma once

#include "../simpleElementGenerator.h"

namespace qReal {
namespace generators {

class WaitForEncoder_gen: public SimpleElementGenerator {
public:
	WaitForEncoder_gen(NxtOSEKRobotGenerator* gen, Id elementId): SimpleElementGenerator(gen, elementId) {
	}
	
protected:
	virtual void generateBodyWithoutNextElementCall();
};

}
}
