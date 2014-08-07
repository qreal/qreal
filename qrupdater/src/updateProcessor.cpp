#include "updateProcessor.h"

using namespace qrUpdater;

UpdateProcessor::UpdateProcessor()
	: mAttempt(0)
	, mHardUpdate(false)
	, mUpdatesFolder("ForwardUpdates/")
	, mCommunicator(new Communicator(this))
	, mDownloader(new Downloader(this))
	, mParser(new XmlDataParser())
	, mUpdateInfo(new UpdateStorage(mUpdatesFolder, this))
{
	try {
		mArgsParser.parse();
	} catch(ArgsParser::BadArgumentsException const &exception) {
		mCommunicator->writeCustomMessage(exception.message());
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

	mAttempt++;
	mDownloader->getUpdateDetails(mArgsParser.detailsUrl());
}

void UpdateProcessor::initConnections()
{
	connect(&mRetryTimer, &QTimer::timeout, this, &UpdateProcessor::startDownloadingProcess);
	connect(&mUpdatesInstaller, &UpdatesInstaller::installsFinished, this, &UpdateProcessor::installingFinished);
	connect(&mUpdatesInstaller, &UpdatesInstaller::selfInstalling, this, &UpdateProcessor::jobDoneQuit);
	connect(mDownloader, &Downloader::detailsLoadError, this, &UpdateProcessor::downloadErrors);
	connect(mDownloader, &Downloader::updatesLoadError, this, &UpdateProcessor::downloadErrors);
	connect(mDownloader, &Downloader::updateDownloaded, this, &UpdateProcessor::fileReady);
	connect(mDownloader, &Downloader::downloadingFinished
			, this, &UpdateProcessor::downloadingFinished, Qt::QueuedConnection);
	connect(mDownloader, &Downloader::detailsDownloaded, mParser, &DetailsParser::processDevice);
	connect(mParser, &DetailsParser::parseFinished, this, &UpdateProcessor::detailsChanged);
}

bool UpdateProcessor::hasNewUpdates(qReal::Version const &newVersion) const
{
	return newVersion > mArgsParser.version();
}

void UpdateProcessor::checkoutPreparedUpdates()
{
	if (!mUpdateInfo->hasPreparedUpdatesInfo()) {
		return;
	}

	mUpdateInfo->loadUpdatesInfo(mArgsParser.units());
	for (Update * const update : mUpdateInfo->preparedUpdates()) {
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
	QProcess * const mainApplication = new QProcess();
	connect(mainApplication, &QProcess::started, this, &UpdateProcessor::jobDoneQuit);
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
	for (Update * const update : mParser->updatesParsed()) {
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

void UpdateProcessor::fileReady(QUrl const &url, QString const &filePath)
{
	mUpdateInfo->saveFileForLater(mParser->update(url), filePath);
}

void UpdateProcessor::downloadingFinished()
{
	if (mHardUpdate) {
		mUpdatesInstaller << mParser->updatesParsed();
		mUpdatesInstaller.installAll();
	} else if (!mRetryTimer.isActive()) { // have no download-errors
		jobDoneQuit();
	}
}

void UpdateProcessor::installingFinished(bool hasSuccess)
{
	restartMainApplication();
	jobDoneQuit();
	Q_UNUSED(hasSuccess);
}

void UpdateProcessor::downloadErrors(QString const &error)
{
	if (mAttempt < maxAttemptsCount) {
		mRetryTimer.start(retryTimerout);
	} else {
		jobDoneQuit();
	}

	Q_UNUSED(error);
}

void UpdateProcessor::jobDoneQuit()
{
	mUpdateInfo->sync();
	QCoreApplication::quit();
}
