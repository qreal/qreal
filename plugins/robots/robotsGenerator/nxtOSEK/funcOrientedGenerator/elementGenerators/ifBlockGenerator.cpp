#include "ifBlockGenerator.h"

using namespace qReal;
using namespace robots::generator;

void IfBlockGenerator::generateMethodBody() {
	IdList outgoingLinks = mNxtGen->mApi->outgoingLinks(mElementId);
	if (outgoingLinks.size() < 2) {
		mNxtGen->mErrorReporter.addError(
				QObject::tr("Less than 2 outgoing elements for if element!")
				, mElementId);
		return;
	}

	//search for arrow with condition
	int conditionArrowNum =
			mNxtGen->mApi->property(outgoingLinks.at(0), "Guard").toString().isEmpty()
			? 1 : 0;
	QString condition = "(" + mNxtGen->mApi->property(mElementId, "Condition").toString() + ")";

	QByteArray conditionOnArrow =
		mNxtGen->mApi->stringProperty(outgoingLinks.at(conditionArrowNum), "Guard").toUtf8();
	if (conditionOnArrow == "меньше 0") {
		condition += " < 0";
	} else if (conditionOnArrow == "больше 0") {
		condition += " > 0";
	} else {
		condition += " == 0";
	}
	
	Id positiveBranchElement = mNxtGen->mApi->to(outgoingLinks.at(conditionArrowNum));
	if (positiveBranchElement == Id::rootId()) {
		QString errorMessage = QObject::tr("If block %1 has no 2 correct branches!"\
					" May be you need to connect one of them to some diagram element.");
		errorMessage.replace("%1", mElementId.toString());
		mNxtGen->mErrorReporter.addError(
				errorMessage
				, mElementId);
		return;
	}
	QString positiveBranchNextElementMethod = mNxtGen->mIdToMethodNameMap[positiveBranchElement.toString()];

	Id negativeBranchElement = mNxtGen->mApi->to(outgoingLinks.at(1 - conditionArrowNum));
	if (negativeBranchElement == Id::rootId()) {
		QString errorMessage = QObject::tr("If block %1 has no 2 correct branches!"\
				" May be you need to connect one of them to some diagram element.");
		errorMessage.replace("%1", mElementId.toString());
		mNxtGen->mErrorReporter.addError(
				errorMessage
				, mElementId);
		return;
	}
	QString negativeBranchNextElementMethod = mNxtGen->mIdToMethodNameMap[negativeBranchElement.toString()];

	mNxtGen->mGeneratedStrings.append(SmartLine("if (" + condition + ") {", mElementId, SmartLine::increase));
	mNxtGen->mGeneratedStrings.append(SmartLine(positiveBranchNextElementMethod + "();", mElementId));
	mNxtGen->mGeneratedStrings.append(SmartLine("} else {", mElementId, SmartLine::increaseDecrease));
	mNxtGen->mGeneratedStrings.append(SmartLine(negativeBranchNextElementMethod + "();", mElementId));
	mNxtGen->mGeneratedStrings.append(SmartLine("}", mElementId, SmartLine::decrease));
}
