#pragma once

#include "../simpleElementGenerator.h"

namespace qReal {
namespace generators {

class NullificationEncoder_gen: public SimpleElementGenerator {
public:
	NullificationEncoder_gen(NxtOSEKRobotGenerator* gen, Id elementId): SimpleElementGenerator(gen, elementId) {
	}
	
protected:
	virtual void generateBodyWithoutNextElementCall();
};

}
}
}
