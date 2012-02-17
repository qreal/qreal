#pragma once

#include "../simpleElementGenerator.h"

namespace robots {
namespace generator {

class WaitForEncoder_gen: public SimpleElementGenerator {
public:
	WaitForEncoder_gen(FuncOrientedGenerator* gen, qReal::Id elementId): SimpleElementGenerator(gen, elementId) {
	}
	
protected:
	virtual void generateBodyWithoutNextElementCall();
};

}
}
