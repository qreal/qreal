#pragma once

#include "../simpleElementGenerator.h"

namespace robots {
namespace generator {

class InitialBlock_gen: public SimpleElementGenerator {
public:
	InitialBlock_gen(FuncOrientedGenerator* gen, qReal::Id elementId): SimpleElementGenerator(gen, elementId) {
	}

protected:	
	virtual void generateBodyWithoutNextElementCall();
};

}
}
