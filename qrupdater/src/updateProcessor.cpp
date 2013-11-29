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

	if (!mUpdateInfo->preparedUpdate()->isInstalling()) {
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
	connect(mDownloader, SIGNAL(detailsLoadError(QString)), this, SLOT(downloadErrors(QString)));
	connect(mDownloader, SIGNAL(updatesLoadError(QString)), this, SLOT(downloadErrors(QString)));
	connect(mDownloader, SIGNAL(updatesDownloaded(QString)), this, SLOT(fileReady(QString)));
	connect(mDownloader, SIGNAL(detailsDownloaded(QIODevice*)), mParser, SLOT(parseDevice(QIODevice*)));
	connect(mParser, SIGNAL(parseFinished()), this, SLOT(detailsChanged()));
}

bool UpdateProcessor::hasNewUpdates(QString const newVersion)
{
	return newVersion > mArgsParser.version();
}

void UpdateProcessor::startSetupProcess(Update *update)
{
	mCommunicator->writeQuitMessage();

}

void UpdateProcessor::checkoutPreparedUpdates()
{
	if (!mUpdateInfo->hasPreparedUpdatesInfo()) {
		return;
	}

	mUpdateInfo->loadUpdateInfo(mArgsParser.units().first());
	if (!hasNewUpdates(mUpdateInfo->preparedUpdate()->version()) || mUpdateInfo->preparedUpdate()->isEmpty()) {
		return;
	}


	startSetupProcess(mUpdateInfo->preparedUpdate());
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

	mParser->changeUnit(mArgsParser.units().first());
	if (!hasNewUpdates(mParser->currentUpdate()->version())) {
		jobDoneQuit();
	}

	mDownloader->getUpdate(mParser->currentUpdate()->url());
}

void UpdateProcessor::fileReady(QString const filePath)
{
	if (mHardUpdate) {
		startSetupProcess(mParser->currentUpdate());
		return;
	}

	mParser->changeUnit(mArgsParser.units().first());
	mUpdateInfo->saveFileForLater(mParser, filePath);
	jobDoneQuit();
}

void UpdateProcessor::updateFinished(bool hasSuccess)
{


	restartMainApplication();
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

