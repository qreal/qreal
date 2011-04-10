#pragma once

#include <QString>
#include <QFile>
#include <QTextStream>
#include <QMap>

namespace Geny {
	class Gemake {
		public:
			Gemake(QString gemakeFilename);
			~Gemake();

			bool init();
			void make();
			QString getTaskFilename(const QString& taskName);

		private:
			QFile makeFile;
			QMap<QString, QString> *filesByTasks;
			QTextStream* inStream;
			QString repoPath;
	};
}
