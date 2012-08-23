#pragma once

#include "../../../../qrkernel/ids.h"
#include "nxtOSEKRobotGenerator.h"
#include "generators/abstractElementGenerator.h"

namespace robots {
namespace generator {

/// Element generator factory that returns generator for diagram element Id.
class ElementGeneratorFactory {
public:
	static AbstractElementGenerator* generator(
			NxtOSEKRobotGenerator *generator
			, qReal::Id const &elementId
			, qrRepo::RepoApi const &api
		);
};

}
}
