#include "debuggerConnector.h"

using namespace qReal;

DebuggerConnector::DebuggerConnector(QString debuggerPath, QString builderPath):
	mThread(new QThread()), mProcess(new QProcess(this)), 
	mDebuggerPath(debuggerPath), mBuilderPath(builderPath)
{
	moveToThread(mThread);
	connect(mProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(readOutput()));
	connect(mProcess, SIGNAL(readyReadStandardError()), this, SLOT(readErrOutput()));
}

DebuggerConnector::~DebuggerConnector() {
	mProcess->closeWriteChannel();
	mProcess->terminate();
	mThread->terminate();
	delete mProcess;
	delete mThread;
}

void DebuggerConnector::run() {
	mThread->start();
	mProcess->start(mDebuggerPath, QStringList());
	mProcess->waitForStarted();
	mProcess->waitForReadyRead();
}

void DebuggerConnector::configure(QString programPath) {
	QString command = "file " + programPath + "\n";
	sendCommand(command);
}

void DebuggerConnector::readOutput() {
	QByteArray out = mProcess->readAllStandardOutput();
	emit readyReadStdOutput(QString(out));
}

void DebuggerConnector::readErrOutput() {
	QByteArray out = mProcess->readAllStandardError();
	emit readyReadErrOutput(QString(out));
}

void DebuggerConnector::sendCommand(QString command) {
	mProcess->write(command.toAscii());  
	mProcess->waitForBytesWritten();
	mProcess->waitForReadyRead();
}

void DebuggerConnector::build(QString filePath) {
	QProcess *gcc = new QProcess(this);
	QStringList args;
	args.append("-g");
	args.append("-o");
	args.append("builded");
	args.append(filePath);
	gcc->start(mBuilderPath, args);
}

void DebuggerConnector::finishProcess() {
	//mProcess->closeWriteChannel();
	mProcess->close();
}
