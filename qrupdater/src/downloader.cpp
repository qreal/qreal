#include "downloader.h"

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
	QString fileName = QFileInfo(url.path()).fileName();
	if (fileName.isEmpty())
		fileName = "update" + QString::number(mLoadedFileIndex++);

	if (QFile::exists(fileName)) {
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
		emit detailsLoadError(qPrintable(reply->errorString()));
	} else {
		emit detailsDownloaded(reply);
	}
}

void Downloader::updatesFileDownloaded(QNetworkReply *reply)
{
	disconnect(&mManager, &QNetworkAccessManager::finished, this, &Downloader::updatesFileDownloaded);
	if (mReply->error()) {
		emit updatesLoadError(qPrintable(reply->errorString()));
		mFile->remove();
	} else {
		mFile->flush();
		mFile->close();
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
		emit downloadingFinished();
		return;
	}

	getUpdate(mFilesToDownload.takeFirst());
}
