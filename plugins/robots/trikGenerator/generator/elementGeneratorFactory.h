#pragma once

#include "../../../../qrkernel/ids.h"
#include "trikRobotGenerator.h"
#include "generators/abstractElementGenerator.h"

namespace robots {
namespace trikGenerator {

/// Element generator factory that returns generator for diagram element Id.
class ElementGeneratorFactory {
public:
	static AbstractElementGenerator* generator(
			TrikRobotGenerator *generator
			, qReal::Id const &elementId
			, qrRepo::RepoApi const &api
		);
};

}
}
