#pragma once

#include <QString>
#include <QObject>

namespace qReal {
	class DebuggerConnector
	{
		public:
			DebuggerConnector(QObject* parent, QString debuggerPath, QString outputFile, QString inputFile);
			~DebuggerConnector();
			
			void run(QString filePath, QStringList args);
			
		private:
			QString mDebuggerPath;
			QString mOutputFile;
			QString mInputFile;
			QObject* mParent;
	};
}
