#pragma once

#include "../simpleElementGenerator.h"

namespace qReal {
namespace generators {

class PlayTone_gen: public SimpleElementGenerator {
public:
	PlayTone_gen(NxtOSEKRobotGenerator* gen, Id elementId): SimpleElementGenerator(gen, elementId) {
	}
	
protected:
	virtual void generateBodyWithoutNextElementCall();
};

}
}
