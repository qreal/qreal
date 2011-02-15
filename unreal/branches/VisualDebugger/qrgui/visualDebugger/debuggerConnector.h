#pragma once

#include <QString>
#include <QThread>
#include <QProcess>

namespace qReal {
	class DebuggerConnector : public QThread
	{
		Q_OBJECT
	
		public:
			DebuggerConnector(QString debuggerPath, QString builderPath);
			~DebuggerConnector();
			
			void run();

		signals:
			void readyReadStdOutput(QString output);
			void readyReadErrOutput(QString output);
			
		public slots:
			void sendCommand(QString command);
			void build(QString filePath);
			void finishProcess();
			void configure(QString programPath);
			
		private:
			QProcess *mProcess;
			QString mDebuggerPath;
			QString mBuilderPath;
			
		private slots:
			void readOutput();
			void readErrOutput();
	};
}
