#include "abstractElementGenerator.h"

#include "../nxtOSEKRobotGenerator.h"

#include "../elementGeneratorFactory.h"

using namespace robots::generator;

AbstractElementGenerator::AbstractElementGenerator(NxtOSEKRobotGenerator *mainGenerator
		, qReal::Id const &elementId): mNxtGen(mainGenerator), mElementId(elementId)
{
}

AbstractElementGenerator::~AbstractElementGenerator()
{
}

void AbstractElementGenerator::createListsForIncomingConnections()
{
	//connects string lists in mGeneratedStringSet with mElementId in mElementToStringListNumbers
	for (int i = 1; i < mNxtGen->api()->incomingConnectedElements(mElementId).size(); i++) {
		mNxtGen->generatedStringSet() << QList<SmartLine>();
		mNxtGen->elementToStringListNumbers()[mElementId.toString()] << mNxtGen->generatedStringSet().size() - 1;
	}
}

bool AbstractElementGenerator::generate()
{
	if (!preGenerationCheck()) {
		return false;
	}

	if (mNxtGen->elementToStringListNumbers().contains(mElementId.toString())) {
		//if we have already observed this element with more than 1 incoming connection

		qReal::Id loopElement = mElementId;
		if (!mNxtGen->previousLoopElements().empty()) {
			loopElement = mNxtGen->previousLoopElementsPop();
		}

		//loopElement must create loop code
		AbstractElementGenerator *loopElementGen = ElementGeneratorFactory::generator(mNxtGen, loopElement, *mNxtGen->api());

		int num = mNxtGen->elementToStringListNumbersPop(loopElement.toString());
		QList<SmartLine> set = mNxtGen->generatedStringSet()[num] + loopElementGen->loopPrefixCode();
		mNxtGen->setGeneratedStringSet(num, set);
		mNxtGen->generatedStringSet() << loopElementGen->loopPostfixCode();

		return true;
	}

	//in case element has more than 1 incoming connection
	//means that element has incoming connections from another elements, we haven`t already observed
	createListsForIncomingConnections();

	return nextElementsGeneration();
}
