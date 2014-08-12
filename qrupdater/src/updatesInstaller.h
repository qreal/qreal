#pragma once

#include <QtCore/QDir>
#include <QtCore/QMap>
#include <QtCore/QTimer>
#include <QtCore/QQueue>
#include <QtCore/QProcess>
#include <QtCore/QCoreApplication>

#include "update.h"

namespace qrUpdater {
/// Installs the queue of updates.
class UpdatesInstaller : public QObject
{
	Q_OBJECT
public:
	explicit UpdatesInstaller(QList<Update *> updates);
	UpdatesInstaller();
	UpdatesInstaller &operator<< (Update *update);
	UpdatesInstaller &operator<< (QList<Update *> updates);

	/// Installes all of updates in the queue.
	void installAll();

	bool hasNoErrors() const;
	bool isEmpty() const;

signals:
	void installsFinished(bool hasNoErrors);
	void selfInstalling();

protected:
	void installNext();
	void replaceExpressions(Update *update);
	void sortQueue();
	static QString installationDirectory();

	bool mHasNoErrors;
	QQueue<Update *> mUpdatesQueue;

protected slots:
	void singleInstallFinished(bool hasNoErrors);
	void startInstallation();
};

}
