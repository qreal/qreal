#pragma once

#include <QProcess>
#include <QObject>

namespace qrRepo
{

	namespace details
	{

		class ExternalClient : public QObject
		{
			Q_OBJECT

		public:
			ExternalClient(QString const &pathToClient);
			ExternalClient(ExternalClient const &other);

			void setPathToClient(QString const &pathToClient);
			void doCheckout(QString const &from, QString const &to) const;
			void doUpdate(QString const &to) const;
			void doCommit(QString const &from) const;
			void doAdd(QString const &what, bool force = true) const;
			void doRemove(QString const &what, bool force = true) const;
			QStringList getNewErrors();

		private slots:
			void processErrors();

		private:
			QString mPathToClient;
			QProcess *mClientProcess;
			QStringList mErrors;
		};
	}
}
