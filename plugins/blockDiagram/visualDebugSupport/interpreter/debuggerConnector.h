#pragma once

#include <QString>
#include <QThread>
#include <QProcess>
#include "../../../qrkernel//settingsManager.h"

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
		void sendCommand(QString command);
		///Terminate debugger process
		void finishProcess();
		///Set program to be debugged
		void configure();
	
	///Signals for transfer debugger connector output to the main application
	signals:
		void readyReadStdOutput(QString output);
		void readyReadErrOutput(QString output);

	private:
		QThread *mThread;
		QProcess *mDebuggerProcess;
		QProcess *mBuilderProcess;
		QString mDebuggerPath;
		QString mBuilderPath;
		QString mBuildedFileName;
		QString mCodeFileName;
		QString mWorkDir;
		bool mHasGccError;

		void setDebuggerPath(QString path);
		void setBuilderPath(QString path);
		void setBuildedFileName(QString name);
		void setCodeFileName(QString name);
		void setWorkDir(QString name);

	private slots:
		///Read debugger output
		void readOutput();
		///Read debugger error output
		void readErrOutput();
		///Read builder output
		void readBuilderErrOutput();
		///Read builder error output
		void readBuilderStdOutput();
};

}
