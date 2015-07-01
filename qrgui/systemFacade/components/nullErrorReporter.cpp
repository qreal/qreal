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

#include "nullErrorReporter.h"

using namespace qReal;

void NullErrorReporter::addInformation(const QString &message, const Id &position)
{
	Q_UNUSED(message)
	Q_UNUSED(position)
}

void NullErrorReporter::addWarning(const QString &message, const Id &position)
{
	Q_UNUSED(message)
	Q_UNUSED(position)
}

void NullErrorReporter::addError(const QString &message, const Id &position)
{
	Q_UNUSED(message)
	Q_UNUSED(position)
	mWereErrors = true;
}

void NullErrorReporter::addCritical(const QString &message, const Id &position)
{
	Q_UNUSED(message)
	Q_UNUSED(position)
	mWereErrors = true;
}

void NullErrorReporter::sendBubblingMessage(const QString &message, int duration, QWidget *parent)
{
	Q_UNUSED(message)
	Q_UNUSED(duration)
	Q_UNUSED(parent)
}

bool NullErrorReporter::wereErrors()
{
	return mWereErrors;
}

void NullErrorReporter::clear()
{
}

void NullErrorReporter::clearErrors()
{
	mWereErrors = false;
}
