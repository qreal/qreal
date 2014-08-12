#pragma once

#include <QtCore/QStringList>
#include <QtCore/QSettings>
#include <QtCore/QFileInfo>
#include <QtCore/QTimer>

#include "argsParser.h"
#include "downloader.h"
#include "xmlDataParser.h"
#include "updateStorage.h"
#include "updatesInstaller.h"
#include "communicator.h"

namespace qrUpdater {

/// Controls update process on the highest level.
class UpdateProcessor : public QObject
{
	Q_OBJECT

public:
	/// Parses application params.
	UpdateProcessor();
	~UpdateProcessor();

	/// Starts all updating processes.
	void startUpdateControl();

private slots:
	void startDownloadingProcess();

private:
	void initConnections();

	/// @return True if new version is newer than current
	bool hasNewUpdates(qReal::Version const &newVersion) const;

	/// Loads prepared update and installs it.
	void checkoutPreparedUpdates();

	/// Restarts main application after installation is finished.
	void restartMainApplication();

	int mAttempt;
	QString mUpdatesFolder;
	QTimer mRetryTimer;
	ArgsParser mArgsParser;
	UpdatesInstaller mUpdatesInstaller;
	Communicator *mCommunicator;
	Downloader *mDownloader;
	DetailsParser *mParser;
	UpdateStorage *mUpdateInfo;

protected slots:
	void detailsChanged();
	void fileReady(QUrl const &url, QString const &filePath);
	void downloadingFinished();
	void installationFinished(bool hasSuccess);
	void downloadErrors(QString const &error = QString());
	void jobDoneQuit();
};

}
