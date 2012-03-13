#pragma once

#include "../simpleElementGenerator.h"

namespace robots {
namespace generator {

class WaitForSonarDistanceGenerator: public SimpleElementGenerator {
public:
	WaitForSonarDistanceGenerator(FuncOrientedGenerator* gen, qReal::Id elementId): SimpleElementGenerator(gen, elementId) {
	}
	
protected:
	virtual void generateBodyWithoutNextElementCall();
};

}
}
