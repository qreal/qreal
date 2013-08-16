#include "commentElementGenerator.h"
#include "../nxtOSEKRobotGenerator.h"

using namespace qReal;
using namespace robots::generator;

CommentElementGenerator::CommentElementGenerator(NxtOSEKRobotGenerator *gen
		, Id const &elementId)
	: AbstractElementGenerator(gen, elementId)
{
}

QList<SmartLine_old> CommentElementGenerator::addLoopCodeInPrefixForm()
{
	return QList<SmartLine_old>();
}

QList<SmartLine_old> CommentElementGenerator::addLoopCodeInPostfixForm()
{
	return QList<SmartLine_old>();
}

bool CommentElementGenerator::preGenerationCheck()
{
	IdList const links = mNxtGen->api()->incomingLinks(mElementId);
	if (!links.isEmpty()) {
		mNxtGen->errorReporter().addError(QObject::tr("The comment block with incomming liks detected!"));
		return false;
	}
	return true;
}

bool CommentElementGenerator::nextElementsGeneration()
{
	return true;
}
