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

#include "error.h"

using namespace qReal::gui;

Error::Error(const QString &message, const Severity &severity, const qReal::Id &position)
		: mMessage(message), mTimestamp(QTime::currentTime()), mSeverity(severity), mPosition(position)
{
}

Error::Severity Error::severity() const
{
	return mSeverity;
}

QString Error::message() const
{
	return mMessage;
}

QString Error::timestamp() const
{
	return "[" + mTimestamp.toString() + "]:";
}

qReal::Id Error::position() const
{
	return mPosition;
}
