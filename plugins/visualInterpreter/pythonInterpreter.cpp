#include "pythonInterpreter.h"
#include "pythonGenerator.h"

#include <QtCore/QFile>

using namespace qReal;

PythonInterpreter::PythonInterpreter(QObject *parent
		, QString const pythonPath
		, QString const scriptPath)
		: QObject(parent)
		, mThread(new QThread())
		, mInterpreterProcess(new QProcess(this))
		, mPythonPath(pythonPath)
		, mScriptPath(scriptPath)
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

void PythonInterpreter::interpret()
{
	QStringList args;
	args.append(mScriptPath);
	if (QFile::exists(mScriptPath)) {
		mInterpreterProcess->start(mPythonPath, args);
		mInterpreterProcess->waitForStarted();
		mInterpreterProcess->waitForReadyRead();
	}
}

void PythonInterpreter::setPythonPath(QString const &path)
{
	mPythonPath = path;
}

void PythonInterpreter::setScriptPath(QString const &path)
{
	mScriptPath = path;
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

	int const semicolumnIndex = output.indexOf(";", pos);
	int const equalsIndex = output.indexOf("=", pos);
	QString const elemName = output.mid(pos, delimeterIndex - pos);
	QString const attrName = output.mid(delimeterIndex + PythonGenerator::delimeter.length()
			, equalsIndex -delimeterIndex - PythonGenerator::delimeter.length());
	QString const value = output.mid(equalsIndex + 1, semicolumnIndex - equalsIndex - 1);

	res.insert(QPair<QString, QString>(elemName, attrName), value);
	pos = semicolumnIndex + 1;
	parseOutput(res, output, pos);
}

void PythonInterpreter::readOutput()
{
	QByteArray const out = mInterpreterProcess->readAllStandardOutput();
	QString const outputString = QString(out);

	QHash<QPair<QString, QString>, QString> output = parseOutput(outputString);
	emit readyReadStdOutput(output);
}

void PythonInterpreter::readErrOutput()
{
	QByteArray const out = mInterpreterProcess->readAllStandardError();
	QString const output = QString(out);
	emit readyReadErrOutput(output);
}
