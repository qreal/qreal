#pragma once

#include "../simpleElementGenerator.h"

namespace robots {
namespace generator {

class WaitForEncoderGenerator: public SimpleElementGenerator {
public:
	WaitForEncoderGenerator(FuncOrientedGenerator* gen, qReal::Id const &elementId): SimpleElementGenerator(gen, elementId) {
	}
	
protected:
	virtual void generateBodyWithoutNextElementCall();
};

}
}
