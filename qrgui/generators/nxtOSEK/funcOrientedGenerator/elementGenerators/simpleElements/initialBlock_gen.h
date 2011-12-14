#pragma once

#include "../simpleElementGenerator.h"

namespace qReal {
namespace generators {
namespace nxtOSEK {

class InitialBlock_gen: public SimpleElementGenerator {
public:
	InitialBlock_gen(FuncOrientedGenerator* gen, Id elementId): SimpleElementGenerator(gen, elementId) {
	}

protected:	
	virtual void generateBodyWithoutNextElementCall();
};

}
}
}
