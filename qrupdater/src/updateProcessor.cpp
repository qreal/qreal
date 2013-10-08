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
	if (!parseParams()) {
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
	mDownloader->getUpdateDetails(mParams.value("-url"));
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

bool UpdateProcessor::parseParams()
{
	int const criticalParamsCount = 3;
	int const argsCount = QCoreApplication::arguments().size();
	if (argsCount - 1 < criticalParamsCount * 2) {
		return false;
	}

	mHardUpdate = QCoreApplication::arguments().contains("-hard", Qt::CaseInsensitive);
	QStringList params;
	params << "-unit" << "-version" << "-url";
	foreach (QString param, params) {
		int index = QCoreApplication::arguments().indexOf(param);
		if (index == -1 || index + 1 >= argsCount)
			return false;

		mParams.insert(param, QCoreApplication::arguments().at(index + 1));
	}

	return true;
}

bool UpdateProcessor::hasNewUpdates(QString const newVersion)
{
	return newVersion > mParams.value("-version");
}

void UpdateProcessor::startSetupProcess(Update *update)
{
	mCommunicator->writeQuitMessage();
	connect(update, SIGNAL(installFinished(bool)), this, SLOT(updateFinished(bool)));
	update->installUpdate();
}

void UpdateProcessor::checkoutPreparedUpdates()
{
	if (!mUpdateInfo->hasPreparedUpdatesInfo()) {
		return;
	}

	mUpdateInfo->loadUpdateInfo(mParams.value("-unit"));
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

	mParser->changeUnit(mParams.value("-unit"));
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

	mParser->changeUnit(mParams.value("-unit"));
	mUpdateInfo->saveFileForLater(mParser, filePath);
	jobDoneQuit();
}

void UpdateProcessor::updateFinished(bool hasSuccess)
{
	if (hasSuccess) {
		if (mUpdateInfo->preparedUpdate()->isInstalled()) {
			mParams.insert("-version", mUpdateInfo->preparedUpdate()->version());
			mUpdateInfo->removePreparedUpdate();
		} else {
			mParams.insert("-version", mParser->currentUpdate()->version());
			mParser->currentUpdate()->clear();
		}
	}

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

