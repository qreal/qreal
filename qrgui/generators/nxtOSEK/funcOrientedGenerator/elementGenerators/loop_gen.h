#pragma once

#include "../abstractElementGenerator.h"

namespace qReal {
namespace generators {
namespace nxtOSEK {

//class for Loop element
class Loop_gen: public AbstractElementGenerator {
public:
	Loop_gen(FuncOrientedGenerator* gen, Id elementId): AbstractElementGenerator(gen, elementId) {
	}
	
protected:
	virtual void generateMethodBody();
};

}
}
}
