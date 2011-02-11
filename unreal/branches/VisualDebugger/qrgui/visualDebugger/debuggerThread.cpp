#include "debuggerThread.h"

using namespace qReal;

DebuggerThread::DebuggerThread() {
	process = new QProcess(this); 
}

void DebuggerThread::run() {
	connect(process,SIGNAL(finished(int)),this,SLOT(quit()));
	process->setStandardOutputFile("output.txt");
	process->start("gdb.exe", QStringList() << "code.c");
	exec();
}
