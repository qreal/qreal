#pragma once

#include "classes/object.h"
#include "difference.h"

#include <QProcess>

namespace qrRepo
{

	namespace details
	{

		enum DiffState
		{
			Added,
			Removed,
			Changed,
			Same
		};

		class DiffProvider
		{

		public:
			DiffProvider(QString const &pathToClient);
			void setPathToClient(QString const &pathToClient);
			QHash<Object*, Difference*> *getDifference(QString const &workingCopy);

		private:
			QString mPathToClient;
			QTextStream mDiffStream;
		};
	}
}
