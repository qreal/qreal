#include "updateProcessor.h"

#include <qrkernel/logging.h>

using namespace qrUpdater;

UpdateProcessor::UpdateProcessor()
	: mAttempt(0)
	, mUpdatesFolder("ForwardUpdates/")
	, mCommunicator(new Communicator(this))
	, mDownloader(new Downloader(this))
	, mParser(new XmlDataParser())
	, mUpdateInfo(new UpdateStorage(mUpdatesFolder, this))
{
	try {
		mArgsParser.parse();
	} catch(ArgsParser::BadArgumentsException const &exception) {
		mCommunicator->writeMessage(exception.message());
	}

	initConnections();
}

UpdateProcessor::~UpdateProcessor()
{
	delete mParser;
}

void UpdateProcessor::startUpdateControl()
{
	if (mArgsParser.mode() == install || mArgsParser.mode() == downloadAndInstall) {
		checkoutPreparedUpdates();
	}

	if (mUpdatesInstaller.isEmpty()) {
		startDownloadingProcess();
	}
}

void UpdateProcessor::startDownloadingProcess()
{
	if (mRetryTimer.isActive()) {
		mRetryTimer.stop();
	}

	++mAttempt;
	QLOG_INFO() << "An attempt #" << mAttempt << "to get details file from"
			<< mArgsParser.detailsUrl() << "...";
	mDownloader->getUpdateDetails(mArgsParser.detailsUrl());
}

void UpdateProcessor::initConnections()
{
	connect(&mRetryTimer, &QTimer::timeout, this, &UpdateProcessor::startDownloadingProcess);
	connect(&mUpdatesInstaller, &UpdatesInstaller::installsFinished, this, &UpdateProcessor::installationFinished);
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
			QLOG_INFO() << "Found unit" << update->unit() << update->version() << "ready for installation";
			mUpdatesInstaller << update;
		}
	}

	if (mUpdatesInstaller.isEmpty()) {
		return;
	}

	mUpdatesInstaller.installAll();
}

void UpdateProcessor::restartMainApplication()
{
	QString const filePath = mArgsParser.pathToApplication();
	QLOG_INFO() << "Restarting" << filePath;
	QString const startPath = QFileInfo(filePath).path();
	QProcess * const mainApplication = new QProcess(this);
	connect(mainApplication, &QProcess::started, this, &UpdateProcessor::jobDoneQuit);
	mainApplication->setWorkingDirectory(startPath);
	mainApplication->startDetached(filePath);
}

void UpdateProcessor::detailsChanged()
{
	if (mParser->hasErrors()) {
		QLOG_ERROR() << "Details parsing accomplished with error";
		downloadErrors();
		return;
	}

	QList<QUrl> filesUrl;
	for (Update * const update : mParser->updatesParsed()) {
		if (mArgsParser.units().contains(update->unit()) && hasNewUpdates(update->version())) {
			QLOG_INFO() << "Found new version!" << update->unit() << update->version().toString();
			filesUrl << update->url();
			if (mArgsParser.mode() == check) {
				mCommunicator->writeMessage(QString("Found %1 of version %2\n")
						.arg(update->unit(), update->version().toString()));
			}
		}
	}

	if (!filesUrl.isEmpty() && (mArgsParser.mode() == download || mArgsParser.mode() == downloadAndInstall)) {
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
	if (mArgsParser.mode() == downloadAndInstall) {
		mUpdatesInstaller << mParser->updatesParsed();
		mUpdatesInstaller.installAll();
	} else if (!mRetryTimer.isActive()) { // have no download-errors
		jobDoneQuit();
	}
}

void UpdateProcessor::installationFinished(bool hasSuccess)
{
	restartMainApplication();
	jobDoneQuit();
	Q_UNUSED(hasSuccess);
}

void UpdateProcessor::downloadErrors(QString const &error)
{
	if (mAttempt < maxAttemptsCount) {
		QLOG_INFO() << "Retrying downloading in 5 min...";
		mRetryTimer.start(retryTimeout);
	} else {
		QLOG_INFO() << "Attempts limit exceeded. Aborting...";
		jobDoneQuit();
	}

	Q_UNUSED(error);
}

void UpdateProcessor::jobDoneQuit()
{
	mUpdateInfo->sync();
	QLOG_INFO() << "Job is done, quiting";
	QCoreApplication::quit();
}
