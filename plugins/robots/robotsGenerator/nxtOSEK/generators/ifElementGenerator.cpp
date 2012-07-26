#include "ifElementGenerator.h"

#include "../nxtOSEKRobotGenerator.h"
#include "../elementGeneratorFactory.h"

using namespace qReal;
using namespace robots::generator;

IfElementGenerator::IfElementGenerator(NxtOSEKRobotGenerator *emboxGen
		, qReal::Id const &elementId): AbstractElementGenerator(emboxGen, elementId)
{
}

QList<SmartLine> IfElementGenerator::addLoopCodeInPrefixForm()
{
	QList<SmartLine> result;

	qReal::Id const logicElementId = mNxtGen->api()->logicalId(mElementId); //TODO
	result << SmartLine("while ("
			+ mNxtGen->api()->property(logicElementId, "Condition").toString()
				+ ") {", mElementId, SmartLine::increase); //TODO
	return result;
}

QList<SmartLine> IfElementGenerator::addLoopCodeInPostfixForm()
{
	QList<SmartLine> result;
	result << SmartLine("}", mElementId, SmartLine::decrease);
	return result;
}

bool IfElementGenerator::preGenerationCheck()
{
	IdList const outgoingLinks = mNxtGen->api()->outgoingLinks(mElementId);

	//TODO: append checking arrows
	return (outgoingLinks.size() == 2);
}

bool IfElementGenerator::generateBranch(int branchNumber)
{
	IdList const outgoingLinks = mNxtGen->api()->outgoingLinks(mElementId);

	Id const branchElement = mNxtGen->api()->to(outgoingLinks.at(branchNumber));
	if (branchElement == Id::rootId()) {
		mNxtGen->errorReporter().addError("If block " + mElementId.toString() + " has no 2 correct branches!"\
				" May be you need to connect one of them to some diagram element.", mElementId);
		return false;
	}

	AbstractElementGenerator* nextBlocksGen = ElementGeneratorFactory::generator(mNxtGen
			, branchElement, *mNxtGen->api());

	mNxtGen->previousElement() = mElementId;

	if (!nextBlocksGen->generate()) {
		return false;
	}
	delete nextBlocksGen;

	return true;
}

QPair<bool, qReal::Id> IfElementGenerator::checkBranchForBackArrows(qReal::Id const &curElementId)
{
	//initial step of checking
	IdList emptyList;
	return checkBranchForBackArrows(curElementId, &emptyList);
}

QPair<bool, qReal::Id> IfElementGenerator::checkBranchForBackArrows(qReal::Id const &curElementId
		, qReal::IdList* checkedElements)
{
	// TODO: Why the hell it is using logical model when in other places there is graphical?
	qReal::Id logicElementId = curElementId;
	if (!mNxtGen->api()->isLogicalElement(curElementId)) {
		logicElementId = mNxtGen->api()->logicalId(curElementId);
	}

	if (checkedElements->contains(logicElementId)) {
		//if we have already observed this element by checkBranchForBackArrows function
		return QPair<bool, qReal::Id>(false, qReal::Id());
	}

	//if we have observed this element and generated code of this element
	foreach (QString observedElementString, mNxtGen->elementToStringListNumbers().keys()) {
		qReal::Id observedElementId = qReal::Id::loadFromString(observedElementString);
		qReal::Id observedElementLogicId = mNxtGen->api()->logicalId(observedElementId);

		if ((logicElementId == observedElementId)
			|| (logicElementId == observedElementLogicId)) {
			return QPair<bool, qReal::Id>(true, logicElementId);
		}
	}

	//add element to list
	checkedElements->append(logicElementId);

	foreach (qReal::Id childId, mNxtGen->api()->outgoingConnectedElements(logicElementId)) {
		if (childId == Id::rootId()) {
			mNxtGen->errorReporter().addError("Link from " + logicElementId.toString() +
					" has no object on its end."\
					" May be you need to connect it to diagram object.", mElementId);
			return QPair<bool, qReal::Id>(false, qReal::Id());
		}

		QPair<bool, qReal::Id> childResult = checkBranchForBackArrows(childId, checkedElements);
		if (childResult.first) {
			return childResult;
		}
	}

	//release element to list
	checkedElements->removeAll(logicElementId);

	return QPair<bool, qReal::Id>(false, qReal::Id());
}

bool IfElementGenerator::nextElementsGeneration()
{
	IdList outgoingLinks = mNxtGen->api()->outgoingLinks(mElementId);
	Q_ASSERT(outgoingLinks.size() == 2);

	//we search for arrow with condition
	qReal::Id const graphicalId = outgoingLinks.at(0);
	qReal::Id const logicalId = mNxtGen->api()->logicalId(graphicalId);
	QVariant const guardProperty = mNxtGen->api()->property(logicalId, "Guard");
	int const conditionArrowNum = guardProperty.toString().isEmpty() ? 1 : 0;

	qReal::Id const logicElementId = mNxtGen->api()->logicalId(mElementId); //TODO

	//TODO: save number of new created list
	QString condition = "(" + mNxtGen->api()->property(logicElementId, "Condition").toString() + ")";
	addNeededCondition(condition, outgoingLinks, conditionArrowNum);

	//check for back arrows
	Id const positiveBranchElement = mNxtGen->api()->to(mNxtGen->api()->logicalId(outgoingLinks.at(conditionArrowNum)));
	if (positiveBranchElement == Id::rootId()) {
		mNxtGen->errorReporter().addError("If block " + mElementId.toString() + " has no 2 correct branches!"\
				" May be you need to connect one of them to some diagram element.", mElementId);
		return false;
	}

	QPair<bool, qReal::Id> const positiveBranchCheck = checkBranchForBackArrows(positiveBranchElement);
	bool const isPositiveBranchReturnsToBackElems = positiveBranchCheck.first;

	Id const negativeBranchElement = mNxtGen->api()->to(outgoingLinks.at(1 - conditionArrowNum));
	if (negativeBranchElement == Id::rootId()) {
		mNxtGen->errorReporter().addError("If block " + mElementId.toString() + " has no 2 correct branches!"\
				" May be you need to connect one of them to some diagram element.", mElementId);
		return false;
	}

	QPair<bool, qReal::Id> const negativeBranchCheck = checkBranchForBackArrows(negativeBranchElement);
	bool const isNegativeBranchReturnsToBackElems = negativeBranchCheck.first;

	if (isPositiveBranchReturnsToBackElems && isNegativeBranchReturnsToBackElems) {
		displaysSuitableError(positiveBranchCheck, negativeBranchCheck);
		return false;
	}

	if (isPositiveBranchReturnsToBackElems != isNegativeBranchReturnsToBackElems) {
		generateIfBlock(isPositiveBranchReturnsToBackElems, conditionArrowNum, condition);
		return true;
	}

	if (!isPositiveBranchReturnsToBackElems && !isNegativeBranchReturnsToBackElems) {
		generateBlockIfElseIs(condition, conditionArrowNum);
	}

	return true;
}

void IfElementGenerator::addNeededCondition(QString &condition, IdList outgoingLinks, int conditionArrowNum)
{
	QByteArray const conditionOnArrow =
		mNxtGen->api()->stringProperty(mNxtGen->api()->logicalId(outgoingLinks.at(conditionArrowNum)), "Guard").toUtf8();
	if (conditionOnArrow == "меньше 0") {
		condition += " < 0";
	} else if (conditionOnArrow == "больше 0") {
		condition += " > 0";
	} else {
		condition += " == 0";
	}
}

void IfElementGenerator::displaysSuitableError(QPair<bool, qReal::Id> const positiveBranchCheck
		, QPair<bool, qReal::Id> const negativeBranchCheck)
{
	if (positiveBranchCheck.second != negativeBranchCheck.second) {
		mNxtGen->errorReporter().addError(
				"This diagram isn't structed diagram,"\
				" because there are IF block with 2 back arrows!", mElementId);
	} else {
	//TODO: repair for case with merged branches
	mNxtGen->errorReporter().addError("Generator JUST does not work in this case.", mElementId);
	}
}

void IfElementGenerator::generateIfBlock(bool isPositiveBranchReturnsToBackElems, int const conditionArrowNum
		, QString condition)
{
	int cycleBlock = isPositiveBranchReturnsToBackElems ? conditionArrowNum : 1 - conditionArrowNum;
	if (conditionArrowNum == cycleBlock) {
		condition = "!" + condition;
	}

	QList<SmartLine> ifBlock;
	ifBlock << SmartLine("if (" + condition + ") {", mElementId, SmartLine::increase);
	ifBlock << SmartLine("break;", mElementId, SmartLine::withoutChange);
	ifBlock << SmartLine("}", mElementId, SmartLine::decrease);
	mNxtGen->generatedStringSet() << ifBlock;
	generateBranch(cycleBlock);

	QList<SmartLine> ifBlockPostfix;
	generateBranch(1 - cycleBlock);
	mNxtGen->generatedStringSet() << ifBlockPostfix;
}

void IfElementGenerator::generateBlockIfElseIs(QString condition, int conditionArrowNum)
{
	QList<SmartLine> ifBlockPrefix;
	ifBlockPrefix << SmartLine("if (" + condition + ") {", mElementId, SmartLine::increase);
	mNxtGen->generatedStringSet() << ifBlockPrefix;

	//generate true/false blocks
	generateBranch(conditionArrowNum);
	QList<SmartLine> elseBlock;
	elseBlock << SmartLine("} else {", mElementId, SmartLine::decreaseOnlyThisLine);
	mNxtGen->generatedStringSet() << elseBlock;
	generateBranch(1 - conditionArrowNum);

	QList<SmartLine> ifBlockPostfix;
	ifBlockPostfix << SmartLine("}", mElementId, SmartLine::decrease);
	mNxtGen->generatedStringSet() << ifBlockPostfix;
}

