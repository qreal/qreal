#pragma once

#include "../abstractElementGenerator.h"

namespace robots {
namespace generator {

//class for Loop element
class Loop_gen: public AbstractElementGenerator {
public:
	Loop_gen(FuncOrientedGenerator* gen, qReal::Id elementId): AbstractElementGenerator(gen, elementId) {
	}
	
protected:
	virtual void generateMethodBody();
};

}
}
