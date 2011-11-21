#pragma once

#include "../simpleElementGenerator.h"

namespace qReal {
namespace generators {

class Beep_gen: public SimpleElementGenerator {
public:
	Beep_gen(NxtOSEKRobotGenerator* gen, Id elementId): SimpleElementGenerator(gen, elementId) {
	}
	
protected:
	virtual void generateBodyWithoutNextElementCall();
};

}
}
