#include "simpleElementGenerator.h"

#include "../../smartLine.h"

using namespace qReal;
using namespace generators;
using namespace nxtOSEK;

void SimpleElementGenerator::generateMethodBody()
{
	generateBodyWithoutNextElementCall();
	
	IdList outgoingConnectedElements = mNxtGen->mApi->outgoingConnectedElements(mElementId);
	if (outgoingConnectedElements.size() > 1) {
		mNxtGen->mErrorReporter.addError("Too many outgoing connections!", mElementId);
		return;
	}

	//if (outgoingConnectedElements.size() == 0) - case of last loop element, for example
	
	if (outgoingConnectedElements.size() == 1) {
		Id nextElement = outgoingConnectedElements.at(0);
		if (!mNxtGen->mApi->isLogicalElement(nextElement))
			nextElement = mNxtGen->mApi->logicalId(nextElement);
		
		QString methodName = mNxtGen->mIdToMethodNameMap[nextElement.toString()];
		mNxtGen->mGeneratedStrings.append(SmartLine(methodName + "();", mElementId));
	}
}
