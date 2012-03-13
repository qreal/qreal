#pragma once

#include "../abstractElementGenerator.h"

namespace robots {
namespace generator {

/// Class for IfBlock element
class IfBlockGenerator: public AbstractElementGenerator {
public:
	IfBlockGenerator(FuncOrientedGenerator* gen, qReal::Id const &elementId): AbstractElementGenerator(gen, elementId) {
	}
	
protected:
	virtual void generateMethodBody();
};

}
}
