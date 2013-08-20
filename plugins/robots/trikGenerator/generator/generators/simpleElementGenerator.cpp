#include "simpleElementGenerator.h"

#include "../trikRobotGenerator.h"
#include "../elementGeneratorFactory.h"
#include "simpleElements/abstractSimpleElementGenerator.h"

using namespace qReal;
using namespace robots::trikGenerator;

SimpleElementGenerator::SimpleElementGenerator(TrikRobotGenerator *trikGenerator
		, qReal::Id const &elementId): AbstractElementGenerator(trikGenerator, elementId)
{
}

QList<SmartLine> SimpleElementGenerator::convertBlockIntoCode()
{
	QList<SmartLine> result;

	qReal::Id const logicElementId = mTrikGenerator->api()->logicalId(mElementId); //TODO

	result = AbstractSimpleElementGenerator::convertedCode(mTrikGenerator, mElementId, logicElementId);
	return result;
}

bool SimpleElementGenerator::nextElementsGeneration()
{
	IdList const outgoingConnectedElements = mTrikGenerator->api()->outgoingConnectedElements(mElementId);
	mTrikGenerator->generatedStringSet() << convertBlockIntoCode();

	if (outgoingConnectedElements.size() == 1) {
		if (outgoingConnectedElements.at(0) == Id::rootId()) {
			mTrikGenerator->errorReporter().addError("Element " + mElementId.toString() + " has no"\
					" correct next element because its link has no end object."\
					" May be you need to connect it to diagram object.", mElementId);
			return false;
		}

		AbstractElementGenerator* const gen = ElementGeneratorFactory::generator(mTrikGenerator
				, outgoingConnectedElements.at(0), *mTrikGenerator->api());
		mTrikGenerator->previousElement() = mElementId;
		gen->generate();
		delete gen;
		return true;
	} else if ((mElementId.element() == "FinalNode") && (outgoingConnectedElements.size() == 0)) {
		return true;
	} else {
		//case of error end of diagram
		mTrikGenerator->errorReporter().addError(QObject::tr("There is no outgoing connected element with no final node!")
				, mElementId);
		return false;
	}

	return true;
}

QList<SmartLine> SimpleElementGenerator::addLoopCodeInPrefixForm()
{
	QList<SmartLine> result;
	result << SmartLine("while (true) {", mElementId, SmartLine::increase);
	return result;
}

QList<SmartLine> SimpleElementGenerator::addLoopCodeInPostfixForm()
{
	QList<SmartLine> result;
	result << SmartLine("}", mElementId, SmartLine::decrease);
	return result;
}

bool SimpleElementGenerator::preGenerationCheck()
{
	IdList const outgoingConnectedElements = mTrikGenerator->api()->outgoingConnectedElements(mElementId);
	if (outgoingConnectedElements.size() > 1) {
		//case of error in diagram
		mTrikGenerator->errorReporter().addError(QObject::tr("There are more than 1 outgoing connected elements with simple robot element!"), mElementId);
		return false;
	}

	return true;
}
