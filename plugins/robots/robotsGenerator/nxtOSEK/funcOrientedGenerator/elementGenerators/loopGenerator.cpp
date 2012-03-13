#include "loopGenerator.h"

#include "../../smartLine.h"

using namespace qReal;
using namespace robots::generator;

void LoopGenerator::generateMethodBody() {
	IdList outgoingLinks = mNxtGen->mApi->outgoingLinks(mElementId);
	if (outgoingLinks.size() < 2) {
		mNxtGen->mErrorReporter.addError(
				QObject::tr("Less than 2 outgoing elements for loop element!")
				, mElementId);
		return;
	}

	int elementConnectedByIterationEdgeNumber = -1;
	int afterLoopElementNumber = -1;

	if (mNxtGen->mApi->stringProperty(outgoingLinks.at(0), "Guard").toUtf8() == "итерация") {
		elementConnectedByIterationEdgeNumber = 0;
		afterLoopElementNumber = 1;
	} else {
		elementConnectedByIterationEdgeNumber = 1;
		afterLoopElementNumber = 0;
	}

	//generate loop
	Id loopNextElement = mNxtGen->mApi->to(outgoingLinks.at(elementConnectedByIterationEdgeNumber));
	if (loopNextElement == Id::rootId()) {
		QString errorMessage = QObject::tr("Loop block %1 has no correct loop branch!"\
					" May be you need to connect it to some diagram element.");
		errorMessage.replace("%1", mElementId.toString());
		mNxtGen->mErrorReporter.addError(
				errorMessage
				, mElementId);
		return;
	}

	QString iterationNumber = mNxtGen->mApi->property(mElementId, "Iterations").toString();
	mNxtGen->mGeneratedStrings.append(SmartLine("for (int __iter__ = ; __iter__ < "
			+ iterationNumber
			+ "; __iter__++)", mElementId));
	mNxtGen->mGeneratedStrings.append(SmartLine("{", mElementId, SmartLine::increase));
	QString methodName_LoopElement = mNxtGen->mIdToMethodNameMap[loopNextElement.toString()];
	mNxtGen->mGeneratedStrings.append(SmartLine(methodName_LoopElement + "();", mElementId));
	mNxtGen->mGeneratedStrings.append(SmartLine("}", mElementId, SmartLine::decrease));

	//generate next blocks
	Id nextBlockElement = mNxtGen->mApi->to(outgoingLinks.at(afterLoopElementNumber));
	if (nextBlockElement == Id::rootId()) {
		QString errorMessage = QObject::tr("Loop block %1 has no correct next block branch!"\
					" May be you need to connect it to some diagram element.");
		errorMessage.replace("%1", mElementId.toString());
		mNxtGen->mErrorReporter.addError(
				errorMessage
				, mElementId);

		return;
	}

	QString methodName_NextElement = mNxtGen->mIdToMethodNameMap[nextBlockElement.toString()];
	mNxtGen->mGeneratedStrings.append(SmartLine(methodName_NextElement + "();", mElementId));
}
