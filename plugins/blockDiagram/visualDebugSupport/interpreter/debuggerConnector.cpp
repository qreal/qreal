#include "debuggerConnector.h"

#include <QFile>
#include <QtCore/QDebug>

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

void DebuggerConnector::setDebuggerPath(QString path)
{
	mDebuggerPath = path;
}

void DebuggerConnector::setBuilderPath(QString path)
{
	mBuilderPath = path;
}

void DebuggerConnector::setBuildedFileName(QString name)
{
	mBuildedFileName = name;
}

void DebuggerConnector::setCodeFileName(QString name)
{
	mCodeFileName = name;
}

void DebuggerConnector::setWorkDir(QString path)
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
	setDebuggerPath(SettingsManager::value("debuggerPath", "gdb").toString());

	if (QFile::exists(mDebuggerPath) || mDebuggerPath == "gdb") {
		mDebuggerProcess->start(mDebuggerPath);
		mDebuggerProcess->waitForStarted();
		//mDebuggerProcess->waitForReadyRead();
	} else {
		emit readyReadErrOutput("Debugger not found. " + mDebuggerPath + " was searched");
	}
}

void DebuggerConnector::configure()
{
	QString command = "file " + mWorkDir + mBuildedFileName + "\n";
	sendCommand(command);
}

void DebuggerConnector::readOutput()
{
	QByteArray out = mDebuggerProcess->readAllStandardOutput();
	QString output = QString(out);
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
	QByteArray out = mDebuggerProcess->readAllStandardError();
	QString output = QString(out);
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
	QByteArray out = mBuilderProcess->readAllStandardError();
	emit readyReadErrOutput("gcc build error:\n" + QString(out));
}

void DebuggerConnector::readBuilderStdOutput()
{
	QByteArray out = mBuilderProcess->readAllStandardOutput();
	emit readyReadErrOutput("gcc build error:\n" + QString(out));
}

void DebuggerConnector::sendCommand(QString command)
{
	mDebuggerProcess->write(command.toAscii());
	//mDebuggerProcess->waitForBytesWritten();
}

void DebuggerConnector::build()
{

	setBuilderPath(SettingsManager::value("builderPath", "gcc").toString());
	setBuildedFileName(SettingsManager::value("buildedFileName", "builded").toString());
	setCodeFileName(SettingsManager::value("codeFileName", "code.c").toString());
	setWorkDir(SettingsManager::value("debugWorkingDirectory", "").toString());

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
				emit readyReadErrOutput("Build failed");
			}
		} else {
			mHasGccError = true;
			emit readyReadErrOutput("Builder not found. " + mBuilderPath + " was searched");
		}
	} else {
		mHasGccError = true;
		emit readyReadErrOutput("Source code file not found. " + mWorkDir + mCodeFileName + " was searched");
	}
}

void DebuggerConnector::finishProcess()
{
	mDebuggerProcess->terminate();
	if (mDebuggerProcess->exitCode() != 0) {
		emit readyReadErrOutput("Debugger closing error");
	} else {
		emit readyReadStdOutput("Debugger was closed successfully");
	}
}
