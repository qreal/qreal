#include "elementGeneratorFactory.h"

//#include "generators/ifElementGenerator.h"
//#include "generators/loopElementGenerator.h"
#include "generators/functionElementGenerator.h"
//#include "generators/commentElementGenerator.h"
#include "generators/simpleElementGenerator.h"

using namespace robots::trikGenerator;

AbstractElementGenerator* ElementGeneratorFactory::generator(
		TrikRobotGenerator *generator
		, qReal::Id const &elementId
		, qrRepo::RepoApi const &api)
{
//	if (elementId.element() == "IfBlock") {
//		return new IfElementGenerator(generator, elementId);
//	} else if (elementId.element() == "Loop") {
//		return new LoopElementGenerator(generator, elementId);
//	} else
	if (elementId.element() == "Function") {
		qReal::Id const logicElementId = api.logicalId(elementId);
		return new FunctionElementGenerator(generator, elementId, api.property(logicElementId, "Init").toBool());
	}
//	else if (elementId.element() == "CommentBlock") {
//		return new CommentElementGenerator(generator, elementId);
//	}

	return new SimpleElementGenerator(generator, elementId);
}
