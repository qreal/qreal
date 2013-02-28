#include <QtCore/QFile>

#include "pythonInterpreter.h"
#include "pythonGenerator.h"

using namespace qReal;

PythonInterpreter::PythonInterpreter(QObject *parent
		, QString const &pythonPath
		 , QString const &reactionScriptPath
		 , QString const &applicationConditionScriptPath)
		: QObject(parent)
		, mThread(new QThread())
		, mInterpreterProcess(new QProcess(NULL))
		, mPythonPath(pythonPath)
		, mReactionScriptPath(reactionScriptPath)
		, mApplicationConditionScriptPath(applicationConditionScriptPath)
		, mApplicationConditionResult(false)
{
	moveToThread(mThread);
	connect(mInterpreterProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(readOutput()));
	connect(mInterpreterProcess, SIGNAL(readyReadStandardError()), this, SLOT(readErrOutput()));
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
		mInterpreterProcess->start(mPythonPath, QStringList() << "-i");
		if (!mInterpreterProcess->waitForStarted()) {
			emit readyReadErrOutput(tr("Python path was set incorrectly"));
			return false;
		}
	}
	return true;
}


bool PythonInterpreter::interpret(bool const isApplicationCondition)
{
	if (!startPythonInterpreterProcess()) {
		return false;
	}

	QString const scriptPath = isApplicationCondition ? mApplicationConditionScriptPath : mReactionScriptPath;
	QString const scriptDir = scriptPath.mid(0, scriptPath.lastIndexOf("/"));
	QString const scriptDirStr = "script_dir = '" + scriptDir + "'\n";
	mInterpreterProcess->write(scriptDirStr.toAscii());

	if (QFile::exists(scriptPath)) {
		QString const execfile = "execfile('" + scriptPath + "')\n";
		mInterpreterProcess->write(execfile.toAscii());

		int const timeout = SettingsManager::value("debuggerTimeout").toInt()/2;
		mInterpreterProcess->waitForReadyRead(timeout);
		if (!isApplicationCondition) {
			return true;
		} else {
			return mApplicationConditionResult;
		}
	}

	return false;
}

void PythonInterpreter::interpretCode(QString const code)
{
	startPythonInterpreterProcess();

	QString const finalCode = "#!/usr/bin/python\n# -*- coding: utf-8 -*-\n" + code + "\n\n";

	mInterpreterProcess->write(finalCode.toAscii());
	mInterpreterProcess->waitForBytesWritten();
}

void PythonInterpreter::terminateProcess()
{
	if (mInterpreterProcess->pid() > 0) {
		mInterpreterProcess->terminate();
	}
}

void PythonInterpreter::setPythonPath(QString const &path)
{
	mPythonPath = path;
}

void PythonInterpreter::setReactionScriptPath(QString const &path)
{
	mReactionScriptPath = path;
}

void PythonInterpreter::setApplicationConditionScriptPath(const QString &path)
{
	mApplicationConditionScriptPath = path;
}

QHash<QPair<QString, QString>, QString> &PythonInterpreter::parseOutput(QString const &output) const
{
	int pos = 0;
	QHash<QPair<QString, QString>, QString> *res = new QHash<QPair<QString, QString>, QString>();
	parseOutput(*res, output, pos);
	return *res;
}

void PythonInterpreter::parseOutput(QHash<QPair<QString, QString>, QString> &res, QString const &output, int &pos) const
{
	int const delimeterIndex = output.indexOf(PythonGenerator::delimeter, pos);
	if (delimeterIndex == -1) {
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

	if (outputString == "True\n") {
		mApplicationConditionResult = true;
	} else if (outputString == "False\n") {
		mApplicationConditionResult = false;
	} else {
		QHash<QPair<QString, QString>, QString> output = parseOutput(outputString);
		emit readyReadStdOutput(output);
	}
}

void PythonInterpreter::readErrOutput()
{
	QByteArray const out = mInterpreterProcess->readAllStandardError();
	QString output = QString(out);
	output = output.replace(">>>", "").trimmed();
	output = output.replace("...", "").trimmed();
	if (!output.isEmpty() && output.indexOf("Python") == -1) {
		emit readyReadErrOutput(output);
	}
}
