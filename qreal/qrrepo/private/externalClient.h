#pragma once

#include <QProcess>

namespace qrRepo
{

	namespace details
	{

		class ExternalClient
		{
		public:
			ExternalClient(QString const &pathToClient);

			void setPathToClient(QString const &pathToClient);
			void doCheckout(QString const &from, QString const &to) const;
			void doUpdate(QString const &to) const;
			void doCommit(QString const &from) const;
			void doAdd(QString const &what, bool force = true) const;
			void doRemove(QString const &what, bool force = true) const;

		private:
			QString mPathToClient;
			QProcess *mClientProcess;
		};
	}
}
