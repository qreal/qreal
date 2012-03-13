#pragma once

#include "../simpleElementGenerator.h"

namespace robots {
namespace generator {

class BeepGenerator: public SimpleElementGenerator {
public:
	BeepGenerator(FuncOrientedGenerator* gen, qReal::Id const &elementId): SimpleElementGenerator(gen, elementId) {
	}
	
protected:
	virtual void generateBodyWithoutNextElementCall();
};

}
}
