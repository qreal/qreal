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

#include "shellBlock.h"

using namespace deployment::blocks;

ShellBlock::ShellBlock(ShellWidget *shellWidget)
	: mProcess(new QProcess(this))
	, mShellWidget(shellWidget)
{
	connect(mProcess, &QProcess::readyReadStandardOutput, [=]() {
		mLastOutput = mProcess->readAllStandardOutput();
		mShellWidget->append(mLastOutput);
	});
	connect(mProcess, &QProcess::readyReadStandardError, [=]() {
		mLastError = mProcess->readAllStandardError();
		mShellWidget->append(mLastError);
	});
	connect(mProcess, static_cast<void (QProcess::*)(int)>(&QProcess::finished), [=]() {
		delete sender();
		emit done(mNextBlockId);
	});
}

void ShellBlock::run()
{
	mProcess->start(processName(), arguments());
}

QString ShellBlock::processName() const
{
	return "bash";
}

QStringList ShellBlock::arguments()
{
	return {};
}
