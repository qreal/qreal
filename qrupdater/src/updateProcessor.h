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
//!
//! @brief The UpdateProcessor class
//! Logic of update process
class UpdateProcessor : public QObject
{
	Q_OBJECT
public:
	//! parsing application params
	UpdateProcessor();
	~UpdateProcessor();

	//! starts all updating process
	void startUpdateControl();

public slots:
	void startDownloadingProcess();

protected:
	void initConnections();
	//! @param newVersion
	//! @return True if new version is newer than current
	bool hasNewUpdates(QString const &newVersion);
	//! loads prepared update and installs it
	void checkoutPreparedUpdates();
	//! restart main application after install finished
	void restartMainApplication();

	static int const retryTimerout = 5 * 60 * 1000;  // new try in 5 min
	static int const maxAttemptsCount = 3;  // 3 times before quit
	int mCurAttempt;
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
	void installingFinished(bool const &hasSuccess);
	void downloadErrors(QString error = QString());
	void jobDoneQuit();
};

}

