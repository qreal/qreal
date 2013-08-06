#pragma once

#include "../../../../../qrkernel/ids.h"
#include "../trikRobotGenerator.h"
#include "simpleElements/abstractSimpleElementGenerator.h"

namespace robots {
namespace trikGenerator {

/// Element generator factory that returns generator for diagram element Id.
class SimpleElementFactory
{
public:
	static AbstractSimpleElementGenerator* generator(QString const elementType);
};

}
}
