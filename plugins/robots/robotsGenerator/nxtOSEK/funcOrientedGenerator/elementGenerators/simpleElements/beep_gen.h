#pragma once

#include "../simpleElementGenerator.h"

namespace robots {
namespace generator {

class Beep_gen: public SimpleElementGenerator {
public:
	Beep_gen(FuncOrientedGenerator* gen, qReal::Id elementId): SimpleElementGenerator(gen, elementId) {
	}
	
protected:
	virtual void generateBodyWithoutNextElementCall();
};

}
}
