#include "loopElementGenerator.h"

#include "../nxtOSEKRobotGenerator.h"
#include "../elementGeneratorFactory.h"

using namespace qReal;
using namespace robots::generator;

LoopElementGenerator::LoopElementGenerator(NxtOSEKRobotGenerator *emboxGen
		, qReal::Id const &elementId): AbstractElementGenerator(emboxGen, elementId)
{
}

bool LoopElementGenerator::nextElementsGeneration()
{
	IdList const outgoingLinks = mNxtGen->api()->outgoingLinks(mElementId);
	Q_ASSERT(outgoingLinks.size() == 2);

	int elementConnectedByIterationEdgeNumber = -1;
	int afterLoopElementNumber = -1;

	if (mNxtGen->api()->stringProperty(mNxtGen->api()->logicalId(outgoingLinks.at(0)), "Guard").toUtf8() == "итерация") {
		elementConnectedByIterationEdgeNumber = 0;
		afterLoopElementNumber = 1;
	} else {
		elementConnectedByIterationEdgeNumber = 1;
		afterLoopElementNumber = 0;
	}

	//generate loop
	Id const loopNextElement = mNxtGen->api()->to(outgoingLinks.at(elementConnectedByIterationEdgeNumber));
	if (loopNextElement == Id::rootId()) {
		mNxtGen->errorReporter().addError("Loop block " + mElementId.toString() + " has no correct loop branch!"\
				" May be you need to connect it to some diagram element.", mElementId);
		return false;
	}

	AbstractElementGenerator* const loopGen = ElementGeneratorFactory::generator(mNxtGen
			, loopNextElement, *mNxtGen->api());

	mNxtGen->previousElement() = mElementId;
	mNxtGen->previousLoopElements().push(mElementId);
	if (!loopGen->generate()) {
		return false;
	}
	delete loopGen;

	//generate next blocks
	Id const nextBlockElement = mNxtGen->api()->to(outgoingLinks.at(afterLoopElementNumber));
	if (nextBlockElement == Id::rootId()) {
		mNxtGen->errorReporter().addError("Loop block " + mElementId.toString() + " has no correct next block branch!"\
				" May be you need to connect it to some diagram element.", mElementId);
		return false;
	}

	AbstractElementGenerator* nextBlocksGen = ElementGeneratorFactory::generator(mNxtGen
			, nextBlockElement, *mNxtGen->api());

	mNxtGen->previousElement() = mElementId;
	mNxtGen->previousLoopElements().push(mElementId);
	if (!nextBlocksGen->generate()) {
		return false;
	}
	delete nextBlocksGen;

	return true;
}

QList<SmartLine> LoopElementGenerator::addLoopCodeInPrefixForm()
{
	QList<SmartLine> result;

	qReal::Id const logicElementId = mNxtGen->api()->logicalId(mElementId); //TODO
	result << SmartLine("for (int __iter__ = 0; __iter__ < "
			+ mNxtGen->api()->property(logicElementId, "Iterations").toString()
				+ "; __iter__++) {", mElementId, SmartLine::increase); //TODO
	return result;
}

QList<SmartLine> LoopElementGenerator::addLoopCodeInPostfixForm()
{
	QList<SmartLine> result;
	result << SmartLine("}", mElementId, SmartLine::decrease);
	return result;
}

bool LoopElementGenerator::preGenerationCheck()
{
	IdList const outgoingLinks = mNxtGen->api()->outgoingLinks(mElementId);

	if ((outgoingLinks.size() != 2) ||
		((mNxtGen->api()->property(mNxtGen->api()->logicalId(outgoingLinks.at(0)), "Guard").toString() == "итерация")
		&& (mNxtGen->api()->property(mNxtGen->api()->logicalId(outgoingLinks.at(1)), "Guard").toString() == "итерация") )
	) {
		return false;
	}

	return true;
}
