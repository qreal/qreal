#pragma once

#include <QProcess>
#include <QObject>

namespace qrRepo
{

	namespace details
	{

		class ExternalClient
		{

		public:
			ExternalClient(QString const &pathToClient);
			ExternalClient(ExternalClient const &other);
			void setPathToClient(QString const &pathToClient);
			bool doCheckout(QString const &from, QString const &to);
			bool doUpdate(QString const &to);
			bool doCommit(QString const &from, QString const &message);
			bool doAdd(QString const &what, bool force = true);
			bool doRemove(QString const &what, bool force = true);
			QString info(QString const &workingDir);
			QString repoUrl(QString const &workingDir);

			QStringList newErrors();


		private:
			void initProcess();
			bool processErrors();

			QString mPathToClient;
			QProcess *mClientProcess;
			QStringList mErrors;
		};
	}
}
