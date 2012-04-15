#pragma once

#include "../simpleElementGenerator.h"

namespace robots {
namespace generator {

class InitialNodeGenerator: public SimpleElementGenerator {
public:
	InitialNodeGenerator(FuncOrientedGenerator* gen, qReal::Id const &elementId): SimpleElementGenerator(gen, elementId) {
	}
	
protected:
	virtual void generateBodyWithoutNextElementCall();
};

}
}
