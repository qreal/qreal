#pragma once

#include "../abstractElementGenerator.h"

namespace qReal {
namespace generators {

//class for IfBlock element
class IfBlock_gen: public AbstractElementGenerator {
public:
	IfBlock_gen(NxtOSEKRobotGenerator* gen, Id elementId): AbstractElementGenerator(gen, elementId) {
	}
	
protected:
	virtual void generateMethodBody();
};

}
}
