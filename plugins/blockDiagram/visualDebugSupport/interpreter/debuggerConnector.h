#pragma once

#include <QString>
#include <QThread>
#include <QProcess>

#include "../../../qrkernel/settingsManager.h"

namespace qReal {

///Debugger connector provides an opportunity to interact with existing 
///textual builders and debuggers (like gcc and gdb)
class DebuggerConnector : public QObject
{
	Q_OBJECT

public:
	explicit DebuggerConnector(QObject *parent);
	~DebuggerConnector();
	
	///Run debugger connector entity in other thread
	void run();
	void startDebugger();
	bool isDebuggerRunning();
	
	///Build executable file from generated sources
	void build();
	
	///Has problems with gcc build process
	bool hasBuildError();
	
	///Send command to the debugger
	void sendCommand(QString const &command);
	
	///Terminate debugger process
	void finishProcess();
	
	///Set program to be debugged
	void configure();

///Signals for transfer debugger connector output to the main application
signals:
	void readyReadStdOutput(QString const &output);
	void readyReadErrOutput(QString const &output);

private slots:
	///Read debugger output
	void readOutput();
	
	///Read debugger error output
	void readErrOutput();
	
	///Read builder output
	void readBuilderErrOutput();
	
	///Read builder error output
	void readBuilderStdOutput();
	
private:
	void setDebuggerPath(QString const &path);
	void setBuilderPath(QString const &path);
	void setBuildedFileName(QString const &name);
	void setCodeFileName(QString const &name);
	void setWorkDir(QString const &name);

	QThread *mThread;
	QProcess *mDebuggerProcess;
	QProcess *mBuilderProcess;
	QString mDebuggerPath;
	QString mBuilderPath;
	QString mBuildedFileName;
	QString mCodeFileName;
	QString mWorkDir;
	bool mHasGccError;
};

}
