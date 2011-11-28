#pragma once

#include "../simpleElementGenerator.h"

namespace qReal {
namespace generators {
namespace nxtOSEKgenerator {

class Function_gen: public SimpleElementGenerator {
public:
	Function_gen(NxtOSEKRobotGenerator* gen, Id elementId): SimpleElementGenerator(gen, elementId) {
	}
	
protected:
	virtual void generateBodyWithoutNextElementCall(); //also adds neccessary variable declarations
	void variableAnalysis(QByteArray const &code);
};

}
}
}
