#pragma once

#include "../abstractElementGenerator.h"

namespace robots {
namespace generator {

//class for IfBlock element
class IfBlock_gen: public AbstractElementGenerator {
public:
	IfBlock_gen(FuncOrientedGenerator* gen, qReal::Id elementId): AbstractElementGenerator(gen, elementId) {
	}
	
protected:
	virtual void generateMethodBody();
};

}
}
