#pragma once

#include "../simpleElementGenerator.h"

namespace qReal {
namespace generators {

class WaitForTouchSensor_gen: public SimpleElementGenerator {
public:
	WaitForTouchSensor_gen(NxtOSEKRobotGenerator* gen, Id elementId): SimpleElementGenerator(gen, elementId) {
	}
	
protected:
	virtual void generateBodyWithoutNextElementCall();
};

}
}
