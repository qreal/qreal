#pragma once

#include <QDir>
#include <QSet>
#include <QMap>
#include <QFile>
#include <QMultiHash>
#include <QTextStream>

#include "message.h"

namespace qReal {
	class Logger
		{
		public:
			Logger(QString const workingDir);
			~Logger();

			void enable();
			void disable();

			void setFlag(flag name, bool arg);
			void setWorkingDir(QString workingDir);

			void log(action performed,
					Id const scene);
			void log(action performed,
					Id const scene, Id const target);
			void log(action performed,
					Id const scene, Id const target,
						QVariant const prevData, QVariant const newData,
							QString const additional);

			void log(Id const scene, Message const message);
			void output();

		private:
			bool pass(Id const scene);
			bool editor(Id const scene);

			void remove(Id const scene);
			void remove(Id const scene, QString const workingDir);

			void write(QString const message, Id const scene);
			void write(QString const message, Id const scene, QString const workingDir);

			QString mWorkingDir;

			bool enabled;
			bool flagsEnabled[4];
			QSet<Id> cleanDiagrams;
			QMap<QString, QFile*> files;
			QMultiHash<Id, QString*> buffer;
	};
}
