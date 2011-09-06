#pragma once

#include "../versionControlSystemInterface.h"
#include "serializerVersioningInterface.h"

#include <QProcess>

namespace qrRepo
{

	namespace details
	{

		class ExternalClient : public VersionControlSystemInterface, public SerializerVersioningInterface
		{

		public:
			ExternalClient(QString const &pathToClient);
			ExternalClient(ExternalClient const &other);
			void setPathToClient(QString const &pathToClient);
			bool doCheckout(QString const &from, QString const &to);
			bool doUpdate(QString const &to);
			bool doCommit(QString const &from, QString const &message);
			bool doCleanUp(QString const &what);
			QString info(QString const &workingDir);
			QString repoUrl(QString const &workingDir);
			int currentRevision(QString const &workingDir);

			QStringList newErrors();

			bool doAdd(QString const &what, bool force = true);
			bool doRemove(QString const &what, bool force = true);

		private:
			QString mPathToClient;
			QProcess *mClientProcess;
			QStringList mErrors;

			void initProcess();
			bool startProcess(QStringList const &args);
			bool checkSvnClientPath();
			bool processErrors();
			bool waitForClient();
		};
	}
}
