/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

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
		return;
	}

	if (!boolProperty("Synchronized")) {
		emit done(mNextBlockId);
	}
}

void ReceiveThreadMessageBlock::receiveMessage(const QString &message)
{
	evalCode(mVariable + " = " + message);
	emit done(mNextBlockId);
}
