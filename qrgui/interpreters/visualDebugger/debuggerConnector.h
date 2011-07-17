#pragma once

#include <QString>
#include <QThread>
#include <QProcess>
#include "../../kernel/settingsManager.h"

namespace qReal {
	class DebuggerConnector : public QObject
	{
		Q_OBJECT

		public:
			DebuggerConnector();
			~DebuggerConnector();

			void run();

			void startDebugger();
			bool hasBuildError();
			bool isDebuggerRunning();
			void sendCommand(QString command);
			void build(QString filePath);
			void finishProcess();
			void configure(QString programPath);

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
			void readOutput();
			void readErrOutput();
			void readBuilderErrOutput();
			void readBuilderStdOutput();
	};
}
