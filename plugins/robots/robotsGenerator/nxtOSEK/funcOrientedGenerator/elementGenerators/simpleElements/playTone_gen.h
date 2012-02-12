#pragma once

#include "../simpleElementGenerator.h"

namespace robots {
namespace generator {

class PlayTone_gen: public SimpleElementGenerator {
public:
	PlayTone_gen(FuncOrientedGenerator* gen, qReal::Id elementId): SimpleElementGenerator(gen, elementId) {
	}
	
protected:
	virtual void generateBodyWithoutNextElementCall();
};

}
}
