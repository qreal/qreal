#include "updateProcessor.h"

UpdateProcessor::UpdateProcessor()
	: mCurAttempt(0)
	, mHardUpdate(false)
	, mUpdatesFolder("ForwardUpdates/")
{
	mCommunicator = new Communicator(this);
	mDownloader = new Downloader(this);
	mParser = new XmlDataParser();
	mUpdateInfo = new UpdateStorage(mUpdatesFolder, this);
	try {
		mArgsParser.parse();
	} catch(ArgsParser::BadArguments const &) {
		mCommunicator->writeHelpMessage();
	}

	initConnections();
}

UpdateProcessor::~UpdateProcessor()
{
	delete mParser;
}

void UpdateProcessor::startUpdateControl()
{
	mCommunicator->readProgramPath();

	checkoutPreparedUpdates();

	if (mUpdatesInstaller.isEmpty()) {
		mCommunicator->writeResumeMessage();
		startDownloadingProcess();
	}
}

void UpdateProcessor::startDownloadingProcess()
{
	if (mRetryTimer.isActive()) {
		mRetryTimer.stop();
	}
	mCurAttempt++;
	mDownloader->getUpdateDetails(mArgsParser.detailsUrl());
}

void UpdateProcessor::initConnections()
{
	connect(&mRetryTimer, SIGNAL(timeout()), this, SLOT(startDownloadingProcess()));
	connect(&mUpdatesInstaller, SIGNAL(installsFinished(bool)), this, SLOT(installingFinished(bool)));
	connect(mDownloader, SIGNAL(detailsLoadError(QString)), this, SLOT(downloadErrors(QString)));
	connect(mDownloader, SIGNAL(updatesLoadError(QString)), this, SLOT(downloadErrors(QString)));
	connect(mDownloader, SIGNAL(updateDownloaded(QUrl,QString)), this, SLOT(fileReady(QUrl,QString)));
	connect(mDownloader, SIGNAL(downloadingFinished()), this, SLOT(downloadingFinished()));
	connect(mDownloader, SIGNAL(detailsDownloaded(QIODevice*)), mParser, SLOT(processDevice(QIODevice*)));
	connect(mParser, SIGNAL(parseFinished()), this, SLOT(detailsChanged()));
}

bool UpdateProcessor::hasNewUpdates(QString const newVersion)
{
	return newVersion > mArgsParser.version();
}

void UpdateProcessor::checkoutPreparedUpdates()
{
	if (!mUpdateInfo->hasPreparedUpdatesInfo()) {
		return;
	}

	mUpdateInfo->loadUpdatesInfo(mArgsParser.units());
	foreach (Update *update, mUpdateInfo->preparedUpdates()) {
		if (hasNewUpdates(update->version())) {
			mUpdatesInstaller << update;
		}
	}

	if (mUpdatesInstaller.isEmpty()) {
		return;
	}
	mCommunicator->writeQuitMessage();
	mUpdatesInstaller.installAll();
}

void UpdateProcessor::restartMainApplication()
{
	QString const filePath = mCommunicator->parentAppPath();
	QString const startPath = QFileInfo(filePath).path();
	QProcess *mainApplication = new QProcess();
	connect(mainApplication, SIGNAL(started()), this, SLOT(jobDoneQuit()));
	mainApplication->setWorkingDirectory(startPath);
	mainApplication->start(filePath);
}

void UpdateProcessor::detailsChanged()
{
	if (mParser->hasErrors()) {
		downloadErrors();
		return;
	}

	QList<QUrl> filesUrl;
	foreach (Update *update, mParser->updatesParsed()) {
		if (mArgsParser.units().contains(update->unit()) && hasNewUpdates(update->version())) {
			filesUrl << update->url();
		}
	}

	if (!filesUrl.isEmpty()) {
		mDownloader->getUpdateFiles(filesUrl);
	} else {
		jobDoneQuit();
	}
}

void UpdateProcessor::fileReady(QUrl url, QString const filePath)
{
	mUpdateInfo->saveFileForLater(mParser->update(url), filePath);
}

void UpdateProcessor::downloadingFinished()
{
	if (mHardUpdate) {
		mUpdatesInstaller << mParser->updatesParsed();
		mUpdatesInstaller.installAll();
	} else {
		jobDoneQuit();
	}
}

void UpdateProcessor::installingFinished(bool hasSuccess)
{
	restartMainApplication();
	jobDoneQuit();
	Q_UNUSED(hasSuccess);
}

void UpdateProcessor::downloadErrors(QString error)
{
	if (mCurAttempt < maxAttemptsCount) {
		mRetryTimer.start(retryTimerout);
	} else {
		jobDoneQuit();
	}

	Q_UNUSED(error);
}

void UpdateProcessor::jobDoneQuit()
{
	QCoreApplication::quit();
}

