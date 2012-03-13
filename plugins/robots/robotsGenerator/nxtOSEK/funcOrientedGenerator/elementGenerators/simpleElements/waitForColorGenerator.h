#pragma once

#include "../simpleElementGenerator.h"

namespace robots {
namespace generator {

class WaitForColorGenerator: public SimpleElementGenerator {
public:
	WaitForColorGenerator(FuncOrientedGenerator* gen, qReal::Id elementId): SimpleElementGenerator(gen, elementId) {
	}
	
protected:
	virtual void generateBodyWithoutNextElementCall();
};

}
}
