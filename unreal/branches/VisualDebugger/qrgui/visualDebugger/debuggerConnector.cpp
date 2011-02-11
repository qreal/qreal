#include <QThread>
#include <QProcess>

#include "debuggerConnector.h"

using namespace qReal;

DebuggerConnector::DebuggerConnector(QObject* parent, QString debuggerPath, QString outputFile, QString inputFile):
	mDebuggerPath(debuggerPath),mOutputFile(outputFile),mInputFile(inputFile),mParent(parent)
{
}

DebuggerConnector::~DebuggerConnector() {
}

void DebuggerConnector::run(QString filePath, QStringList args) {
	QThread *thread = new QThread();
	QProcess *process = new QProcess(mParent);
	QStringList *argsList = new QStringList();
	argsList->push_back(filePath);
	argsList->append(args);
	process->setStandardInputFile(mInputFile);
	process->setStandardOutputFile(mOutputFile);
	process->start(mDebuggerPath,*argsList);
	process->start("calc.exe");
}
