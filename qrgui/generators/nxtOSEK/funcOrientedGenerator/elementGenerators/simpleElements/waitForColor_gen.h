#pragma once

#include "../simpleElementGenerator.h"

namespace qReal {
namespace generators {
namespace nxtOSEK {

class WaitForColor_gen: public SimpleElementGenerator {
public:
	WaitForColor_gen(FuncOrientedGenerator* gen, Id elementId): SimpleElementGenerator(gen, elementId) {
	}
	
protected:
	virtual void generateBodyWithoutNextElementCall();
};

}
}
}
