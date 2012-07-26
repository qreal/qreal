#pragma once

#include "../../../../../qrkernel/ids.h"
#include "../nxtOSEKRobotGenerator.h"
#include "simpleElements/abstractsimpleelementgenerator.h"

namespace robots {
namespace generator {

/// Element generator factory that returns generator for diagram element Id.
class SimpleElementFactory {
public:
	static AbstractSimpleElementGenerator* generator(QString const elementType);
};

}
}
