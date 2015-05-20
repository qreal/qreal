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

#include "twoDShell.h"

#include <QtCore/QFile>

using namespace trik::robotModel::twoD::parts;

Shell::Shell(const kitBase::robotModel::DeviceInfo &info
		, const kitBase::robotModel::PortInfo &port)
	: robotModel::parts::TrikShell(info, port)
{
}

void Shell::runCommand(const QString &command)
{
	Q_UNUSED(command)
}

void Shell::say(const QString &text)
{
	Q_UNUSED(text)
}

void Shell::writeToFile(const QString &filePath, const QString &text)
{
	QFile out(filePath);
	out.open(QIODevice::WriteOnly | QIODevice::Append);
	out.write(text.toUtf8());
}

void Shell::removeFile(const QString &filePath)
{
	QFile out(filePath);
	out.remove();
}
