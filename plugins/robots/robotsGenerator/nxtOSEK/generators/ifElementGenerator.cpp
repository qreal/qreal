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
	qReal::Id const logicElementId = mNxtGen->api()->logicalId(mElementId); //TODO
	QString condition = mNxtGen->api()->property(logicElementId, "Condition").toString();
	condition = replaceSensorAndEncoderVariables(condition);

	return QList<SmartLine>() << SmartLine("while (" + condition + ") {"
			, mElementId, SmartLine::increase); //TODO;
}

QList<SmartLine> IfElementGenerator::addLoopCodeInPostfixForm()
{
	QList<SmartLine> result;
	result << SmartLine("}", mElementId, SmartLine::decrease);
	return result;
}

bool IfElementGenerator::preGenerationCheck()
{
	//TODO: Remove this copy-paste from interpreter!

	// In correct case exactly 2 of this 3 would be non-null
	Id trueBlock, falseBlock, nonMarkedBlock;
	IdList const links = mNxtGen->api()->outgoingLinks(mElementId);

	if (links.size() != 2) {
		error(QObject::tr("There must be exactly TWO links outgoing from if block"));
		return false;
	}

	foreach (Id const &linkId, links) {
		Id const targetBlock = mNxtGen->api()->otherEntityFromLink(linkId, mElementId);
		qReal::Id const logicalLinkId = mNxtGen->api()->logicalId(linkId);

		if (targetBlock.isNull() || targetBlock == Id::rootId()) {
			error(QObject::tr("Outgoing link is not connected"));
			return false;
		}

		QString const condition = mNxtGen->api()->property(logicalLinkId, "Guard").toString().toLower();
		if (condition == QString::fromUtf8("истина")) {
			if (trueBlock.isNull()) {
				trueBlock = targetBlock;
			} else {
				error(QObject::tr("Two links marked with 'true' found"));
				return false;
			}
		} else if (condition == QString::fromUtf8("ложь")) {
			if (falseBlock.isNull()) {
				falseBlock = targetBlock;
			} else {
				error(QObject::tr("Two links marked with 'false' found"));
				return false;
			}
		} else {
			if (nonMarkedBlock.isNull()) {
				nonMarkedBlock = targetBlock;
			} else {
				error(QObject::tr("There must be a link with property \"Guard\" set to one of the conditions"));
				return false;
			}
		}
	}

	// Now we have correctly placed links with correct guards. Determining who is who
	if (trueBlock.isNull()) {
		mTrueId = nonMarkedBlock;
		mFalseId = falseBlock;
	} else if (falseBlock.isNull()) {
		mTrueId = trueBlock;
		mFalseId = nonMarkedBlock;
	} else if (nonMarkedBlock.isNull()) {
		mTrueId = trueBlock;
		mFalseId = falseBlock;
	}

	return true;
}

bool IfElementGenerator::generateBranch(Id const &branchElement)
{
	AbstractElementGenerator* nextBlocksGen = ElementGeneratorFactory::generator(mNxtGen
			, branchElement, *mNxtGen->api());

	mNxtGen->currentGenerator()->previousElement() = mElementId;

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
	foreach (QString const &observedElementString
			, mNxtGen->currentGenerator()->elementToStringListNumbers().keys())
	{
		qReal::Id observedElementId = qReal::Id::loadFromString(observedElementString);
		qReal::Id observedElementLogicId = mNxtGen->api()->logicalId(observedElementId);

		if ((logicElementId == observedElementId)
			|| (logicElementId == observedElementLogicId)) {
			return QPair<bool, qReal::Id>(true, logicElementId);
		}
	}

	//add element to list
	checkedElements->append(logicElementId);

	foreach (qReal::Id const &childId, mNxtGen->api()->outgoingConnectedElements(logicElementId)) {
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

	qReal::Id const logicElementId = mNxtGen->api()->logicalId(mElementId); //TODO

	// TODO: save number of new created list
	QString const rawCondition = mNxtGen->api()->property(logicElementId, "Condition").toString();
	QString const condition = replaceSensorAndEncoderVariables(rawCondition);

	bool isPositiveBranchReturnsToBackElems = false;
	bool isNegativeBranchReturnsToBackElems = false;

	if (!areOutgoingLinksCorrect(
			isPositiveBranchReturnsToBackElems
			, isNegativeBranchReturnsToBackElems))
	{
		return false;
	}

	if (isPositiveBranchReturnsToBackElems != isNegativeBranchReturnsToBackElems) {
		generateIfBlock(isPositiveBranchReturnsToBackElems, condition);
		return true;
	}

	if (!isPositiveBranchReturnsToBackElems && !isNegativeBranchReturnsToBackElems) {
		generateBlockIfElseIs(condition);
	}

	return true;
}

bool IfElementGenerator::areOutgoingLinksCorrect(
		bool &isPositiveBranchReturnsToBackElems
		, bool &isNegativeBranchReturnsToBackElems
		)
{
	QPair<bool, qReal::Id> const positiveBranchCheck = checkBranchForBackArrows(mTrueId);
	isPositiveBranchReturnsToBackElems = positiveBranchCheck.first;

	QPair<bool, qReal::Id> const negativeBranchCheck = checkBranchForBackArrows(mFalseId);
	isNegativeBranchReturnsToBackElems = negativeBranchCheck.first;

	if (isPositiveBranchReturnsToBackElems && isNegativeBranchReturnsToBackElems) {
		displaysSuitableError(positiveBranchCheck, negativeBranchCheck);
		return false;
	}

	return true;
}

void IfElementGenerator::displaysSuitableError(QPair<bool, qReal::Id> const positiveBranchCheck
		, QPair<bool, qReal::Id> const negativeBranchCheck)
{
	if (positiveBranchCheck.second != negativeBranchCheck.second) {
		mNxtGen->errorReporter().addError(
				QObject::tr("This diagram isn't structed diagram,"\
				" because there are IF block with 2 back arrows!"), mElementId);
	} else {
		//TODO: repair for case with merged branches
		mNxtGen->errorReporter().addError(QObject::tr("Generator JUST does not work in this case."), mElementId);
	}
}

void IfElementGenerator::generateIfBlock(bool isPositiveBranchReturnsToBackElems
		, QString const &rawCondition)
{
	Id const cycleBlock = isPositiveBranchReturnsToBackElems ? mTrueId : mFalseId;
	Id const nonCycleBlock = isPositiveBranchReturnsToBackElems ? mFalseId : mTrueId;

	QString const condition = isPositiveBranchReturnsToBackElems
			? QString("!(%1)").arg(rawCondition) : rawCondition;

	QList<SmartLine> ifBlock;
	ifBlock << SmartLine("if (" + condition + ") {", mElementId, SmartLine::increase);
	ifBlock << SmartLine("break;", mElementId, SmartLine::withoutChange);
	ifBlock << SmartLine("}", mElementId, SmartLine::decrease);
	mNxtGen->currentGenerator()->generatedStringSet() << ifBlock;
	generateBranch(cycleBlock);

	QList<SmartLine> ifBlockPostfix;
	generateBranch(nonCycleBlock);
	mNxtGen->currentGenerator()->generatedStringSet() << ifBlockPostfix;
}

void IfElementGenerator::generateBlockIfElseIs(QString const &condition)
{
	QList<SmartLine> ifBlockPrefix;
	ifBlockPrefix << SmartLine("if (" + condition + ") {", mElementId, SmartLine::increase);
	mNxtGen->currentGenerator()->generatedStringSet() << ifBlockPrefix;

	// generate true/false blocks
	generateBranch(mTrueId);
	QList<SmartLine> elseBlock;
	elseBlock << SmartLine("} else {", mElementId, SmartLine::decreaseOnlyThisLine);
	mNxtGen->currentGenerator()->generatedStringSet() << elseBlock;
	generateBranch(mFalseId);

	QList<SmartLine> ifBlockPostfix;
	ifBlockPostfix << SmartLine("}", mElementId, SmartLine::decrease);
	mNxtGen->currentGenerator()->generatedStringSet() << ifBlockPostfix;
}
