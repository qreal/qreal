#include "sendThreadMessageBlock.h"

using namespace qReal::interpretation::blocks;

void SendThreadMessageBlock::run()
{
	const QString thread = stringProperty("Thread");
	if (thread.isEmpty()) {
		error(tr("Need to specify a receiving thread in 'Thread' property"));
		return;
	}

	sendMessage(thread, eval<QString>("Message"));
	emit done(mNextBlockId);
}
