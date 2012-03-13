#pragma once

#include "../abstractElementGenerator.h"

namespace robots {
namespace generator {

/// Class for Loop element
class LoopGenerator: public AbstractElementGenerator {
public:
	LoopGenerator(FuncOrientedGenerator* gen, qReal::Id const &elementId): AbstractElementGenerator(gen, elementId) {
	}
	
protected:
	virtual void generateMethodBody();
};

}
}
