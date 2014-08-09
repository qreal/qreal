#include "downloader.h"

#include <qrkernel/logging.h>

using namespace qrUpdater;

Downloader::Downloader(QObject *parent)
	: QObject(parent)
	, mLoadedFileIndex(0)
	, mReply(nullptr)
	, mFile(nullptr)
{
}

void Downloader::getUpdateDetails(QUrl const &url)
{
	connect(&mManager, &QNetworkAccessManager::finished, this, &Downloader::detailsFileDownloaded);
	sendRequest(url);
}

void Downloader::getUpdate(QUrl const &url) throw(CreateFileException)
{
	QLOG_INFO() << "Starting downloading update from" << url.toString() << "...";
	QString fileName = QFileInfo(url.path()).fileName();
	if (fileName.isEmpty()) {
		fileName = "update" + QString::number(mLoadedFileIndex++);
	}

	QLOG_INFO() << "Saving it to" << fileName;
	if (QFile::exists(fileName)) {
		QLOG_INFO() << "Removing old file with such name";
		QFile::remove(fileName);
	}

	mFile = new QFile(fileName);
	if (!mFile->open(QIODevice::WriteOnly)) {
		delete mFile;
		mFile = nullptr;
		throw CreateFileException();
	}

	startFileDownloading(url);
}

void Downloader::getUpdateFiles(QList<QUrl> const &urls)
{
	mFilesToDownload.append(urls);
	downloadNext();
}

void Downloader::detailsFileDownloaded(QNetworkReply *reply)
{
	disconnect(&mManager, &QNetworkAccessManager::finished, this, &Downloader::detailsFileDownloaded);
	if (reply->error()) {
		QLOG_ERROR() << "Getting information about updates failed: " + reply->errorString();
		emit detailsLoadError(reply->errorString());
	} else {
		emit detailsDownloaded(reply);
	}
}

void Downloader::updatesFileDownloaded(QNetworkReply *reply)
{
	disconnect(&mManager, &QNetworkAccessManager::finished, this, &Downloader::updatesFileDownloaded);
	if (mReply->error()) {
		QLOG_ERROR() << "Update" << mFile->fileName() << "downloading error:" << reply->errorString();
		emit updatesLoadError(reply->errorString());
		mFile->remove();
	} else {
		mFile->flush();
		mFile->close();
		QLOG_INFO() << "Update" << reply->request().url() << "downloaded successfully";
		emit updateDownloaded(reply->request().url(), QFileInfo(*mFile).filePath());
	}

	mReply->deleteLater();
	mReply = nullptr;
	delete mFile;
	downloadNext();
}

void Downloader::fileReadyRead()
{
	if (mFile) {
		mFile->write(mReply->readAll());
	}
}

void Downloader::sendRequest(QUrl const &url)
{
	QNetworkRequest request(url);
	mReply = mManager.get(request);
}

void Downloader::startFileDownloading(QUrl const &url)
{
	connect(&mManager, &QNetworkAccessManager::finished, this, &Downloader::updatesFileDownloaded);
	sendRequest(url);
	connect(mReply, &QNetworkReply::readyRead, this, &Downloader::fileReadyRead, Qt::UniqueConnection);
}

void Downloader::downloadNext()
{
	if (mFilesToDownload.isEmpty()) {
		QLOG_INFO() << "Downloading finished";
		emit downloadingFinished();
		return;
	}

	try {
		getUpdate(mFilesToDownload.takeFirst());
	} catch (CreateFileException const &) {
		emit updatesLoadError("Could not open file for writing");
	}
}
