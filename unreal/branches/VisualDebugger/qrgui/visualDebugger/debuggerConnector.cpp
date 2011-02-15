#include "debuggerConnector.h"

using namespace qReal;

DebuggerConnector::DebuggerConnector(QString debuggerPath, QString builderPath): 
	mDebuggerPath(debuggerPath), mBuilderPath(builderPath)
{
	mProcess = new QProcess(this);
	connect(mProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(readOutput()));
	connect(mProcess, SIGNAL(readyReadStandardError()), this, SLOT(readErrOutput()));
	//moveToThread(this); 
}

DebuggerConnector::~DebuggerConnector() {
	mProcess->closeWriteChannel();
	mProcess->terminate();
	delete mProcess;
}

void DebuggerConnector::run() {
	mProcess->start(mDebuggerPath, QStringList());
	mProcess->waitForStarted();
}

void DebuggerConnector::configure(QString programPath) {
	QString command = "file ";
	command.append(programPath);
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
	mProcess->closeWriteChannel();
	mProcess->terminate();
}
