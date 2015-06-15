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

#include "shell.h"

#include <qrutils/inFile.h>

using namespace trik::robotModel::real::parts;
using namespace kitBase::robotModel;

Shell::Shell(const DeviceInfo &info, const PortInfo &port
		, utils::TcpRobotCommunicator &tcpRobotCommunicator)
	: robotModel::parts::TrikShell(info, port)
	, mRobotCommunicator(tcpRobotCommunicator)
{
	connect(&mRobotCommunicator, &utils::TcpRobotCommunicator::printText, this, &Shell::textPrinted);
}

void Shell::say(const QString &text)
{
	const QString pathToCommand = ":/trikQts/templates/say.t";
	const QString directCommand = utils::InFile::readAll(pathToCommand)
			.replace("@@TEXT@@", "\"" + text + "\"") + "script.run();";

	mRobotCommunicator.runDirectCommand(directCommand);
}

void Shell::runCommand(const QString &command)
{
	const QString pathToCommand = ":/trikQts/templates/system.t";
	const QString directCommand = utils::InFile::readAll(pathToCommand)
			.replace("@@COMMAND@@", command) + "script.run();";

	mRobotCommunicator.runDirectCommand(directCommand);
}

void Shell::runCode(const QString &code)
{
	mRobotCommunicator.runDirectCommand(code);
}

void Shell::writeToFile(const QString &filePath, const QString &text)
{
	const QString pathToCommand = ":/trikQts/templates/files/writeFile.t";
	const QString directCommand = utils::InFile::readAll(pathToCommand)
			.replace("@@FILE@@", filePath).replace("@@TEXT@@", "\"" + text + "\"") + "script.run();";

	mRobotCommunicator.runDirectCommand(directCommand);
}

void Shell::removeFile(const QString &filePath)
{
	const QString pathToCommand = ":/trikQts/templates/files/removeFile.t";
	const QString directCommand = utils::InFile::readAll(pathToCommand)
			.replace("@@FILE@@", filePath) + "script.run();";

	mRobotCommunicator.runDirectCommand(directCommand);
}

void Shell::print(const QString &text)
{
	const QString pathToCommand = ":/trikQts/templates/functions/print.t";
	const QString directCommand = utils::InFile::readAll(pathToCommand)
			.replace("@@ARGUMENT@@", "\"" + text + "\"") + ";script.run();";

	mRobotCommunicator.runDirectCommand(directCommand);
}
