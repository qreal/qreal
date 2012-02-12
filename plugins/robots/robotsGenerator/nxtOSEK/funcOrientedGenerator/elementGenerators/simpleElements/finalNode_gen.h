#pragma once

#include "../simpleElementGenerator.h"

namespace robots {
namespace generator {

class FinalNode_gen: public SimpleElementGenerator {
public:
	FinalNode_gen(FuncOrientedGenerator* gen, qReal::Id elementId): SimpleElementGenerator(gen, elementId) {
	}
	
protected:
	virtual void generateBodyWithoutNextElementCall();
};

}
}
