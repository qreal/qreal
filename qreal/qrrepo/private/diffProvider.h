#pragma once

#include "classes/object.h"
#include "difference.h"

#include <QProcess>

namespace qrRepo
{

	namespace details
	{

		class DiffProvider
		{

		public:
			DiffProvider(QString const &pathToClient);
			void setPathToClient(QString const &pathToClient);
			QHash<qReal::Id, Difference*> *getDifference(QHash<qReal::Id, Object*> const &repoObjects,
								  QHash<qReal::Id, Object*> const &workingCopyObjects);

		private:
			QString mPathToClient;
			QTextStream mDiffStream;
			QStringList mErrors;
		};
	}
}
