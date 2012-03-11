#pragma once

#include "../simpleElementGenerator.h"

namespace robots {
namespace generator {

class WaitForColor_gen: public SimpleElementGenerator {
public:
	WaitForColor_gen(FuncOrientedGenerator* gen, qReal::Id elementId): SimpleElementGenerator(gen, elementId) {
	}
	
protected:
	virtual void generateBodyWithoutNextElementCall();
};

}
}
