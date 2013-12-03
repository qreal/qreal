#pragma once

#include <QtCore/QDir>
#include <QtCore/QMap>
#include <QtCore/QTimer>
#include <QtCore/QQueue>
#include <QtCore/QProcess>
#include <QtCore/QCoreApplication>

#include "update.h"

namespace qrUpdater {
//! @class UpdatesInstaller installs queue of updates
class UpdatesInstaller : public QObject
{
	Q_OBJECT
public:
	explicit UpdatesInstaller(QList<Update *> updates);
	UpdatesInstaller();
	void operator<< (Update *update);
	void operator<< (QList<Update *> updates);

	//! installes all of updates in queue
	void installAll();

	bool hasNoErrors() const;
	bool isEmpty() const;

signals:
	void installsFinished(bool hasNoErrors);
	void selfInstalling();

protected:
	void installNext();
	void replaceExpressions(Update *update);
	static QString getInstallDir();

	bool mHasNoErrors;
	//! Time for main program could fully unload resources
	static int const delay = 1000;
	QQueue<Update *> mUpdatesQueue;

protected slots:
	void singleInstallFinished(bool hasNoErrors);
	void startInstallation();
};

}
