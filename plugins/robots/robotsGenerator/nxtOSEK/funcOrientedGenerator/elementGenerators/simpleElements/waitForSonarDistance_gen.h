#pragma once

#include "../simpleElementGenerator.h"

namespace robots {
namespace generator {

class WaitForSonarDistance_gen: public SimpleElementGenerator {
public:
	WaitForSonarDistance_gen(FuncOrientedGenerator* gen, qReal::Id elementId): SimpleElementGenerator(gen, elementId) {
	}
	
protected:
	virtual void generateBodyWithoutNextElementCall();
};

}
}
