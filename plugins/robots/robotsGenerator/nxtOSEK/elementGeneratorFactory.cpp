#include "elementGeneratorFactory.h"

#include "generators/ifElementGenerator.h"
#include "generators/loopElementGenerator.h"
#include "generators/functionElementGenerator.h"
#include "generators/simpleElementGenerator.h"

using namespace robots::generator;

AbstractElementGenerator* ElementGeneratorFactory::generator(
		NxtOSEKRobotGenerator *generator
		, qReal::Id elementId
		, qrRepo::RepoApi const &api)
{
	if (elementId.element() == "IfBlock") {
		return new IfElementGenerator(generator, elementId);
	} else if (elementId.element() == "Loop") {
		return new LoopElementGenerator(generator, elementId);
	} else if (elementId.element() == "Function") {
		qReal::Id const logicElementId = api.logicalId(elementId);
		return new FunctionElementGenerator(generator, elementId, api.property(logicElementId, "Init").toBool());
	}

	return new SimpleElementGenerator(generator, elementId);
}
