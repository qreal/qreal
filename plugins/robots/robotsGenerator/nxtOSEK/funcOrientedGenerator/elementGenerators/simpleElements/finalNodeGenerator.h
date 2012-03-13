#pragma once

#include "../simpleElementGenerator.h"

namespace robots {
namespace generator {

class FinalNodeGenerator: public SimpleElementGenerator {
public:
	FinalNodeGenerator(FuncOrientedGenerator* gen, qReal::Id elementId): SimpleElementGenerator(gen, elementId) {
	}
	
protected:
	virtual void generateBodyWithoutNextElementCall();
};

}
}
