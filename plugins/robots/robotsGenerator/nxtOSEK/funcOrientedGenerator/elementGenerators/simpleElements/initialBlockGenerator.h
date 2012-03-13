#pragma once

#include "../simpleElementGenerator.h"

namespace robots {
namespace generator {

class InitialBlockGenerator: public SimpleElementGenerator {
public:
	InitialBlockGenerator(FuncOrientedGenerator* gen, qReal::Id const &elementId): SimpleElementGenerator(gen, elementId) {
	}

protected:	
	virtual void generateBodyWithoutNextElementCall();
};

}
}
