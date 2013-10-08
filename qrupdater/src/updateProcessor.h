#pragma once

#include <QtCore/QCoreApplication>
#include <QtCore/QStringList>
#include <QtCore/QSettings>
#include <QtCore/QFileInfo>
#include <QtCore/QProcess>
#include <QtCore/QTimer>

#include "downloader.h"
#include "xmlDataParser.h"
#include "updateStorage.h"
#include "communicator.h"

//!
//! @brief The UpdateProcessor class
//! Logic of update process, makes updates
//! and interaction with main application
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

	//! @return True if arguments are correct, false otherwise
	bool parseParams();

	//! @param newVersion
	//! @return True if new version is newer than current
	bool hasNewUpdates(QString const newVersion);
	//! update install
	void startSetupProcess(Update *update);
	//! loads prepared update and installs it
	void checkoutPreparedUpdates();
	//! restart main application after install finished
	void restartMainApplication();

	static int const retryTimerout = 5 * 60 * 1000;
	static int const maxAttemptsCount = 3;
	int mCurAttempt;
	bool mHardUpdate;
	QString mUpdatesFolder;
	QTimer mRetryTimer;
	QMap<QString, QString> mParams;
	Communicator *mCommunicator;
	Downloader *mDownloader;
	DetailsParser *mParser;
	UpdateStorage *mUpdateInfo;

protected slots:
	void detailsChanged();
	void fileReady(QString const filePath);
	void updateFinished(bool hasSuccess);
	void downloadErrors(QString error = QString());
	void jobDoneQuit();
};

