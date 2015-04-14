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

#include "consoleErrorReporter.h"

using namespace qReal;

void ConsoleErrorReporter::addInformation(const QString &message, const Id &position)
{
	Q_UNUSED(position)
	qDebug() << message;
}

void ConsoleErrorReporter::addWarning(const QString &message, const Id &position)
{
	Q_UNUSED(position)
	qWarning() << message;
}

void ConsoleErrorReporter::addError(const QString &message, const Id &position)
{
	Q_UNUSED(position)
	qDebug() << QObject::tr("Error:") << message;
	mWereErrors = true;
}

void ConsoleErrorReporter::addCritical(const QString &message, const Id &position)
{
	Q_UNUSED(position)
	qCritical() << message;
	mWereErrors = true;
}

bool ConsoleErrorReporter::wereErrors()
{
	return mWereErrors;
}

void ConsoleErrorReporter::clear()
{
}

void ConsoleErrorReporter::clearErrors()
{
	mWereErrors = false;
}
