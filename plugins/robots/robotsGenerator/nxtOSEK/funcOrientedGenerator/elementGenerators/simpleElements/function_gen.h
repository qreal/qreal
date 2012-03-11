#pragma once

#include "../simpleElementGenerator.h"

namespace robots {
namespace generator {

class Function_gen: public SimpleElementGenerator {
public:
	Function_gen(FuncOrientedGenerator* gen, qReal::Id elementId): SimpleElementGenerator(gen, elementId) {
	}
	
protected:
	virtual void generateBodyWithoutNextElementCall(); //also adds neccessary variable declarations
	void variableAnalysis(QByteArray const &code);
};

}
}
