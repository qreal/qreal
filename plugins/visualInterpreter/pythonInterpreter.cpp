#include "pythonInterpreter.h"
#include "pythonGenerator.h"
#include "../../qrutils/outFile.h"

using namespace qReal;

PythonInterpreter::PythonInterpreter(QObject *parent
		, QString const &pythonPath
		, QString const &tempScriptPath)
		: QObject(parent)
		, mThread(new QThread())
		, mInterpreterProcess(new QProcess(NULL))
		, mPythonPath(pythonPath)
		, mTempScriptPath(tempScriptPath)
		, mPythonCodeProcessed(false)
		, mApplicationConditionResult(false)
		, mErrorOccured(false)
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
		mInterpreterProcess->write(scriptDirStr.toAscii());
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
		mInterpreterProcess->write(command.toAscii());
	} else {
		mInterpreterProcess->write(actualCode.toAscii());
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
	if (mInterpreterProcess->pid() > 0) {
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
	int const delimeterIndex = output.indexOf(PythonGenerator::delimeter, pos);
	if (output.contains(">>>")) {
		while (output.indexOf(">>>", pos + 1) < delimeterIndex) {
			pos = output.indexOf(">>>", pos + 1);
		}
		pos += 4;
	}

	QHash<QPair<QString, QString>, QString> *res = new QHash<QPair<QString, QString>, QString>();
	parseOutput(*res, output, pos);
	return *res;
}

void PythonInterpreter::parseOutput(QHash<QPair<QString, QString>, QString> &res, QString const &output, int &pos) const
{
	int const delimeterIndex = output.indexOf(PythonGenerator::delimeter, pos);
	if (delimeterIndex == -1 || output.indexOf("';") == -1) {
		return;
	}

	int const semicolumnIndex = output.indexOf("';", pos);
	int const equalsIndex = output.indexOf("='", pos);
	QString const elemName = output.mid(pos, delimeterIndex - pos);
	QString const attrName = output.mid(delimeterIndex + PythonGenerator::delimeter.length()
			, equalsIndex -delimeterIndex - PythonGenerator::delimeter.length());
	QString const value = output.mid(equalsIndex + 2, semicolumnIndex - equalsIndex - 2);

	res.insert(QPair<QString, QString>(elemName, attrName), value);
	pos = semicolumnIndex + 2;
	parseOutput(res, output, pos);
}

void PythonInterpreter::readOutput()
{
	QByteArray const out = mInterpreterProcess->readAllStandardOutput();
	QString const outputString = QString(out);
	if (outputString.isEmpty()) {
		return;
	}

	mErrorOccured = false;
	if (outputString == "True\n") {
		mApplicationConditionResult = true;
		continueStep();
	} else if (outputString == "False\n") {
		mApplicationConditionResult = false;
		continueStep();
	} else {
		QHash<QPair<QString, QString>, QString> output = parseOutput(outputString);
		if (!output.isEmpty()) {
			emit readyReadStdOutput(output);
		} else {
			QString errorOutput = outputString;
			errorOutput = errorOutput.replace(">>>", "").trimmed();
			errorOutput = errorOutput.replace("...", "").trimmed();
			if (!errorOutput.isEmpty() && errorOutput.indexOf("Python") == -1) {
				mErrorOccured = true;
				emit readyReadErrOutput(errorOutput);
			}
		}
	}
}
