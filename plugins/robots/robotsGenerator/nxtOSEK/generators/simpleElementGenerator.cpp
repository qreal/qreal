#include "simpleElementGenerator.h"

#include "../nxtOSEKRobotGenerator.h"
#include "../elementGeneratorFactory.h"
#include "simpleElements/abstractsimpleelementgenerator.h"

using namespace qReal;
using namespace robots::generator;

SimpleElementGenerator::SimpleElementGenerator(NxtOSEKRobotGenerator *emboxGen
		, qReal::Id const &elementId): AbstractElementGenerator(emboxGen, elementId)
{
}

QList<SmartLine> SimpleElementGenerator::convertBlockIntoCode()
{
	QList<SmartLine> result;

	qReal::Id const logicElementId = mNxtGen->api()->logicalId(mElementId); //TODO

	result = AbstractSimpleElementGenerator::convertedCode(mNxtGen, mElementId, logicElementId);
	return result;
}

bool SimpleElementGenerator::nextElementsGeneration()
{
	IdList const outgoingConnectedElements = mNxtGen->api()->outgoingConnectedElements(mElementId);
	mNxtGen->generatedStringSet() << convertBlockIntoCode();
	mNxtGen->initCode().append(mInitCode);

	if (outgoingConnectedElements.size() == 1) {
		if (outgoingConnectedElements.at(0) == Id::rootId()) {
			mNxtGen->errorReporter().addError("Element " + mElementId.toString() + " has no"\
					" correct next element because its link has no end object."\
					" May be you need to connect it to diagram object.", mElementId);
			return false;
		}

		AbstractElementGenerator* const gen = ElementGeneratorFactory::generator(mNxtGen
				, outgoingConnectedElements.at(0), *mNxtGen->api());
		mNxtGen->previousElement() = mElementId;
		gen->generate();
		delete gen;
		return true;
	} else if ((mElementId.element() == "FinalNode") && (outgoingConnectedElements.size() == 0)) {
		return true;
	} else {
		//case of error end of diagram
		mNxtGen->errorReporter().addError(QObject::tr("There is no outgoing connected element with no final node!")
				, mElementId);
		return false;
	}

	return true;
}

QList<SmartLine> SimpleElementGenerator::loopPrefixCode()
{
	QList<SmartLine> result;
	result << SmartLine("while (true) {", mElementId, SmartLine::increase);
	return result;
}

QList<SmartLine> SimpleElementGenerator::loopPostfixCode()
{
	QList<SmartLine> result;
	result << SmartLine("}", mElementId, SmartLine::decrease);
	return result;
}

bool SimpleElementGenerator::preGenerationCheck()
{
	IdList const outgoingConnectedElements = mNxtGen->api()->outgoingConnectedElements(mElementId);
	if (outgoingConnectedElements.size() > 1) {
		//case of error in diagram
		mNxtGen->errorReporter().addError(QObject::tr("There are more than 1 outgoing connected elements with simple robot element!"), mElementId);
		return false;
	}

	return true;
}
