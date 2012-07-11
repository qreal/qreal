#include "abstractElementGenerator.h"

#include "../nxtOSEKRobotGenerator.h"

#include "../elementGeneratorFactory.h"

using namespace robots::generator;

AbstractElementGenerator::AbstractElementGenerator(NxtOSEKRobotGenerator *mainGenerator,
		qReal::Id const &elementId): mNxtGen(mainGenerator), mElementId(elementId)
{
}

AbstractElementGenerator::~AbstractElementGenerator()
{
}

void AbstractElementGenerator::createListsForIncomingConnections()
{
	//connects string lists in mGeneratedStringSet with mElementId in mElementToStringListNumbers
	for (int i = 1; i < mNxtGen->mApi->incomingConnectedElements(mElementId).size(); i++) {
		mNxtGen->mGeneratedStringSet << QList<SmartLine>();
		mNxtGen->mElementToStringListNumbers[mElementId.toString()] << mNxtGen->mGeneratedStringSet.size() - 1;
	}
}

bool AbstractElementGenerator::generate()
{
	if (!preGenerationCheck())
		return false;

	if (mNxtGen->mElementToStringListNumbers.contains(mElementId.toString())) {
		//if we have already observed this element with more than 1 incoming connection

		qReal::Id loopElement = mElementId;
		if (!mNxtGen->mPreviousLoopElements.empty()){
			loopElement = mNxtGen->mPreviousLoopElements.pop();
		}
		//loopElement must create loop code
		AbstractElementGenerator *loopElementGen = ElementGeneratorFactory::generator(mNxtGen, loopElement, *mNxtGen->mApi);

		mNxtGen->mGeneratedStringSet[mNxtGen->mElementToStringListNumbers[loopElement.toString()].pop()]
			+= loopElementGen->loopPrefixCode();

		mNxtGen->mGeneratedStringSet << loopElementGen->loopPostfixCode();

		return true;
}

	//in case element has more than 1 incoming connection
	//means that element has incoming connections from another elements, we haven`t already observed
	createListsForIncomingConnections();

	return nextElementsGeneration();
}
