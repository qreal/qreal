#pragma once

#include "../simpleElementGenerator.h"

namespace robots {
namespace generator {

class NullificationEncoderGenerator: public SimpleElementGenerator {
public:
	NullificationEncoderGenerator(FuncOrientedGenerator* gen, qReal::Id elementId): SimpleElementGenerator(gen, elementId) {
	}
	
protected:
	virtual void generateBodyWithoutNextElementCall();
};

}
}
