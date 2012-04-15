#pragma once

#include "../simpleElementGenerator.h"

namespace robots {
namespace generator {

class FunctionGenerator: public SimpleElementGenerator {
public:
	FunctionGenerator(FuncOrientedGenerator* gen, qReal::Id const &elementId): SimpleElementGenerator(gen, elementId) {
	}
	
protected:
	virtual void generateBodyWithoutNextElementCall(); //also adds neccessary variable declarations
	void variableAnalysis(QByteArray const &code);
};

}
}
