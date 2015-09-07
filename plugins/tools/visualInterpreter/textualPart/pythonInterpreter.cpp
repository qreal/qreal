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

#include "pythonInterpreter.h"
#include "textCodeGenerator.h"
#include <qrutils/outFile.h>

using namespace qReal;

PythonInterpreter::PythonInterpreter(QObject *parent
		, QString const &pythonPath
		, QString const &tempScriptPath)
		: TextCodeInterpreter(parent)
		, mThread(new QThread())
		, mInterpreterProcess(new QProcess(nullptr))
		, mPythonPath(pythonPath)
		, mTempScriptPath(tempScriptPath)
		, mPythonCodeProcessed(false)
{
	moveToThread(mThread);
	connect(mInterpreterProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(readOutput()));
}

PythonInterpreter::~PythonInterpreter()
{
	mInterpreterProcess->closeWriteChannel();
	mInterpreterProcess->terminate();
	mThread->terminate();

	delete mInterpreterProcess;
	delete mThread;
}

bool PythonInterpreter::startPythonInterpreterProcess()
{
	if (mInterpreterProcess->pid() == 0) {
		mInterpreterProcess->setProcessChannelMode(QProcess::MergedChannels);

		mInterpreterProcess->start(mPythonPath, QStringList() << "-i");
		if (!mInterpreterProcess->waitForStarted()) {
			emit readyReadErrOutput(tr("Python path was set incorrectly"));
			return false;
		}

		QString const scriptDir = mTempScriptPath.mid(0, mTempScriptPath.lastIndexOf("/"));
		QString const scriptDirStr = "__script_dir__ = '" + scriptDir + "'\n";
		mInterpreterProcess->write(scriptDirStr.toLatin1());
		mInterpreterProcess->waitForBytesWritten();
	}
	return true;
}

void PythonInterpreter::deleteTempFile()
{
	QFile(mTempScriptPath).remove();
}

bool PythonInterpreter::interpret(QString const &code, CodeType const codeType)
{
	if (!startPythonInterpreterProcess()) {
		return false;
	}

	mPythonCodeProcessed = false;

	QString actualCode = code + "\n\n";
	if (codeType == initialization) {
		actualCode = "#!/usr/bin/python\n# -*- coding: utf-8 -*-\n\n" + actualCode;
	}

	if (codeType != applicationCondition) {
		utils::OutFile out(mTempScriptPath);
		out() << actualCode;
		out().flush();
		QString const command = "execfile('" + mTempScriptPath + "')\n";
		mInterpreterProcess->write(command.toLatin1());
	} else {
		mInterpreterProcess->write(actualCode.toLatin1());
	}

	mInterpreterProcess->waitForBytesWritten();

	if (codeType != initialization) {
		while (!mPythonCodeProcessed) {
			mInterpreterProcess->waitForReadyRead(-1);
		}
	}

	if (codeType == applicationCondition) {
		return mApplicationConditionResult && !mErrorOccured;
	} else {
		return !mErrorOccured;
	}
}

void PythonInterpreter::terminateProcess()
{
	if (mInterpreterProcess->pid()) {
		mInterpreterProcess->terminate();
		deleteTempFile();
	}
}

void PythonInterpreter::continueStep()
{
	mPythonCodeProcessed = true;
}

void PythonInterpreter::setPythonPath(QString const &path)
{
	mPythonPath = path;
}

void PythonInterpreter::setTempScriptPath(const QString &path)
{
	mTempScriptPath = path;
}

QHash<QPair<QString, QString>, QString> &PythonInterpreter::parseOutput(QString const &output) const
{
	int pos = 0;
	int const delimeterIndex = output.indexOf(TextCodeGenerator::delimeter, pos);
	if (output.contains(">>>") && output.indexOf(">>>", pos) < delimeterIndex) {
		while (output.indexOf(">>>", pos + 1) < delimeterIndex) {
			pos = output.indexOf(">>>", pos + 1);
		}
		pos += 4;
	}

	QHash<QPair<QString, QString>, QString> *res = new QHash<QPair<QString, QString>, QString>();
	TextCodeInterpreter::parseOutput(*res, output, pos);
	return *res;
}

void PythonInterpreter::readOutput()
{
	QByteArray const out = mInterpreterProcess->readAllStandardOutput();
	QString const outputString = QString(out);

	QString reducedOutput = QString(outputString);
	reducedOutput = reducedOutput.replace(">>>", "").trimmed();
	reducedOutput = reducedOutput.replace("...", "").trimmed();

	if (reducedOutput.isEmpty()) {
		return;
	}

	mErrorOccured = false;
	if (reducedOutput == "True\n" || reducedOutput == "True") {
		mApplicationConditionResult = true;
		continueStep();
	} else if (reducedOutput == "False\n"  || reducedOutput == "False") {
		mApplicationConditionResult = false;
		continueStep();
	} else if (reducedOutput == "empty reaction") {
		continueStep();
	} else {
		QHash<QPair<QString, QString>, QString> output = parseOutput(outputString);
		if (!output.isEmpty()) {
			emit readyReadStdOutput(output, TextCodeInterpreter::python);
		} else {
			if (!reducedOutput.isEmpty() && reducedOutput.indexOf("Python") == -1
					&& reducedOutput.indexOf("copyright") == -1)
			{
				mErrorOccured = true;
				emit readyReadErrOutput(reducedOutput);
			}
		}
	}
}
