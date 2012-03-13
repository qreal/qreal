#pragma once

#include "../abstractElementGenerator.h"

namespace robots {
namespace generator {

//class for IfBlock element
class IfBlockGenerator: public AbstractElementGenerator {
public:
	IfBlockGenerator(FuncOrientedGenerator* gen, qReal::Id elementId): AbstractElementGenerator(gen, elementId) {
	}
	
protected:
	virtual void generateMethodBody();
};

}
}
