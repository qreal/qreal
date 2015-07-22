/* Copyright 2015 QReal Research Group
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

#include "emptyShell.h"

using namespace trik::qts;

EmptyShell::EmptyShell(const kitBase::robotModel::DeviceInfo &info
		, const kitBase::robotModel::PortInfo &port)
	: TrikShell(info, port)
{
}

void EmptyShell::print(const QString &text)
{
	emit textPrinted(text);
}

void EmptyShell::runCommand(const QString &command)
{
	Q_UNUSED(command)
}

void EmptyShell::runCode(const QString &code)
{
	Q_UNUSED(code)
}

void EmptyShell::say(const QString &text)
{
	Q_UNUSED(text)
}

void EmptyShell::writeToFile(const QString &filePath, const QString &text)
{
	Q_UNUSED(filePath)
	Q_UNUSED(text)
}

void EmptyShell::removeFile(const QString &filePath)
{
	Q_UNUSED(filePath)
}

