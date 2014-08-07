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

	static int const retryTimerout = 5 * 60 * 1000;  // new try in 5 min
	static int const maxAttemptsCount = 3;  // 3 times before quit
	int mAttempt;
	bool mHardUpdate;
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
	void installingFinished(bool hasSuccess);
	void downloadErrors(QString error = QString());
	void jobDoneQuit();
};

}

