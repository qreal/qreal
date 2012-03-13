#pragma once

#include "../simpleElementGenerator.h"

namespace robots {
namespace generator {

class BeepGenerator: public SimpleElementGenerator {
public:
	BeepGenerator(FuncOrientedGenerator* gen, qReal::Id elementId): SimpleElementGenerator(gen, elementId) {
	}
	
protected:
	virtual void generateBodyWithoutNextElementCall();
};

}
}
