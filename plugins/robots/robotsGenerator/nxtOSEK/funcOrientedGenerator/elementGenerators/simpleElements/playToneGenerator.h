#pragma once

#include "../simpleElementGenerator.h"

namespace robots {
namespace generator {

class PlayToneGenerator: public SimpleElementGenerator {
public:
	PlayToneGenerator(FuncOrientedGenerator* gen, qReal::Id const &elementId): SimpleElementGenerator(gen, elementId) {
	}
	
protected:
	virtual void generateBodyWithoutNextElementCall();
};

}
}
