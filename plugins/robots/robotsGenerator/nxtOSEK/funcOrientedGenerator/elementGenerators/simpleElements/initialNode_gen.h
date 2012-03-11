#pragma once

#include "../simpleElementGenerator.h"

namespace robots {
namespace generator {

class InitialNode_gen: public SimpleElementGenerator {
public:
	InitialNode_gen(FuncOrientedGenerator* gen, qReal::Id elementId): SimpleElementGenerator(gen, elementId) {
	}
	
protected:
	virtual void generateBodyWithoutNextElementCall();
};

}
}
