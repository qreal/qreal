#pragma once

#include "../simpleElementGenerator.h"

namespace robots {
namespace generator {

class NullificationEncoder_gen: public SimpleElementGenerator {
public:
	NullificationEncoder_gen(FuncOrientedGenerator* gen, qReal::Id elementId): SimpleElementGenerator(gen, elementId) {
	}
	
protected:
	virtual void generateBodyWithoutNextElementCall();
};

}
}
