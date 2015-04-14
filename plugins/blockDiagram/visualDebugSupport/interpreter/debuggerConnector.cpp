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

#include <QtCore/QFile>

#include "debuggerConnector.h"

using namespace qReal;

DebuggerConnector::DebuggerConnector(QObject *parent)
		: QObject(parent)
		, mThread(new QThread())
		, mDebuggerProcess(new QProcess(this))
		, mBuilderProcess(new QProcess(this))
		, mDebuggerPath("gdb.exe")
		, mBuilderPath("gcc.exe")
		, mBuildedFileName("builded.exe")
		, mCodeFileName("code.c")
		, mWorkDir("")
		, mHasGccError(false)
{
	moveToThread(mThread);
	connect(mDebuggerProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(readOutput()));
	connect(mDebuggerProcess, SIGNAL(readyReadStandardError()), this, SLOT(readErrOutput()));
	connect(mBuilderProcess, SIGNAL(readyReadStandardError()), this, SLOT(readBuilderErrOutput()));
	connect(mBuilderProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(readBuilderStdOutput()));
}

DebuggerConnector::~DebuggerConnector()
{
	mDebuggerProcess->closeWriteChannel();
	mDebuggerProcess->terminate();
	mThread->terminate();
	delete mDebuggerProcess;
	delete mBuilderProcess;
	delete mThread;
}

bool DebuggerConnector::hasBuildError()
{
	return mHasGccError;
}

bool DebuggerConnector::isDebuggerRunning()
{
	return mDebuggerProcess->state() == QProcess::Running;
}

void DebuggerConnector::setDebuggerPath(QString const &path)
{
	mDebuggerPath = path;
}

void DebuggerConnector::setBuilderPath(QString const &path)
{
	mBuilderPath = path;
}

void DebuggerConnector::setBuildedFileName(QString const &name)
{
	mBuildedFileName = name;
}

void DebuggerConnector::setCodeFileName(QString const &name)
{
	mCodeFileName = name;
}

void DebuggerConnector::setWorkDir(QString const &path)
{
	if (path != "") {
		mWorkDir = path + "/";
	}
}

void DebuggerConnector::run()
{
	if (!mThread->isRunning()) {
		mThread->start();
	}
}

void DebuggerConnector::startDebugger()
{
	setDebuggerPath(SettingsManager::value("debuggerPath").toString());

	if (QFile::exists(mDebuggerPath) || mDebuggerPath == "gdb") {
		mDebuggerProcess->start(mDebuggerPath);
		mDebuggerProcess->waitForStarted();
		//mDebuggerProcess->waitForReadyRead();
	} else {
		emit readyReadErrOutput(tr("Debugger not found. ") + mDebuggerPath + tr(" was searched"));
	}
}

void DebuggerConnector::configure()
{
	QString const command = "file " + mWorkDir + mBuildedFileName + "\n";
	sendCommand(command);
}

void DebuggerConnector::readOutput()
{
	QByteArray const out = mDebuggerProcess->readAllStandardOutput();
	QString const output = QString(out);
	int index = output.indexOf("(gdb)");
	if (index > 0) {
		emit readyReadStdOutput(output.mid(0, index));
	} else {
		if (index != 0) {
			emit readyReadStdOutput(output);
		}
	}
	//mDebuggerProcess->waitForReadyRead();
}

void DebuggerConnector::readErrOutput()
{
	QByteArray const out = mDebuggerProcess->readAllStandardError();
	QString const output = QString(out);
	int index = output.indexOf("(gdb)");
	if (index > 0) {
		emit readyReadErrOutput(output.mid(0, index));
	} else {
		if (index != 0) {
			emit readyReadErrOutput(output);
		}
	}
}

void DebuggerConnector::readBuilderErrOutput()
{
	QByteArray const out = mBuilderProcess->readAllStandardError();
	emit readyReadErrOutput(tr("gcc build error:\n") + QString(out));
}

void DebuggerConnector::readBuilderStdOutput()
{
	QByteArray const out = mBuilderProcess->readAllStandardOutput();
	emit readyReadErrOutput(tr("gcc build error:\n") + QString(out));
}

void DebuggerConnector::sendCommand(QString const &command)
{
	mDebuggerProcess->write(command.toLatin1());
	//mDebuggerProcess->waitForBytesWritten();
}

void DebuggerConnector::build()
{
	setBuilderPath(SettingsManager::value("builderPath").toString());
	setBuildedFileName(SettingsManager::value("buildedFileName").toString());
	setCodeFileName(SettingsManager::value("codeFileName").toString());
	setWorkDir(SettingsManager::value("debugWorkingDirectory").toString());

	QStringList args;
	args.append("-g");
	args.append("-o");
	args.append(mWorkDir + mBuildedFileName);
	args.append(mWorkDir + mCodeFileName);

	if (QFile::exists(mWorkDir + mCodeFileName)) {
		if (QFile::exists(mBuilderPath) || mBuilderPath == "gcc") {
			mBuilderProcess->start(mBuilderPath, args);
			mBuilderProcess->waitForStarted();
			mBuilderProcess->waitForReadyRead();
			if (mBuilderProcess->exitCode() != 0) {
				mHasGccError = true;
				emit readyReadErrOutput(tr("Build failed"));
			}
		} else {
			mHasGccError = true;
			emit readyReadErrOutput(tr("Builder not found. ")
					+ mBuilderPath + tr(" was searched"));
		}
	} else {
		mHasGccError = true;
		emit readyReadErrOutput(tr("Source code file not found. ") + mWorkDir + mCodeFileName + tr(" was searched"));
	}
}

void DebuggerConnector::finishProcess()
{
	mDebuggerProcess->terminate();
	if (mDebuggerProcess->exitCode() != 0) {
		emit readyReadErrOutput(tr("Debugger closing error"));
	} else {
		emit readyReadStdOutput(tr("Debugger was closed successfully"));
	}
}
