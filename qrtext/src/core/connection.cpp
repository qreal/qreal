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

#include "qrtext/core/connection.h"

using namespace qrtext::core;

Connection::Connection(int absolutePosition, int line, int column, const qReal::Id &id, const QString &propertyName)
	: mAbsolutePosition(absolutePosition)
	, mLine(line)
	, mColumn(column)
	, mId(id)
	, mPropertyName(propertyName)
{
}

Connection::Connection()
{
}

int Connection::absolutePosition() const
{
	return mAbsolutePosition;
}

int Connection::line() const
{
	return mLine;
}

int Connection::column() const
{
	return mColumn;
}

const qReal::Id &Connection::id() const
{
	return mId;
}

const QString &Connection::propertyName() const
{
	return mPropertyName;
}
