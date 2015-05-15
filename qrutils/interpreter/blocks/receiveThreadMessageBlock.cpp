#include "receiveThreadMessageBlock.h"

#include <qrutils/interpreter/thread.h>

using namespace qReal::interpretation::blocks;

void ReceiveThreadMessageBlock::run()
{
	mVariable = stringProperty("Variable");
	if (mVariable.isEmpty()) {
		error(tr("Need to specify variable which will contain received message"));
		return;
	}

	QString message;
	if (mThread->getMessage(message)) {
		receiveMessage(message);
	}
}

void ReceiveThreadMessageBlock::receiveMessage(const QString &message)
{
	evalCode(mVariable + " = " + message);
	emit done(mNextBlockId);
}
