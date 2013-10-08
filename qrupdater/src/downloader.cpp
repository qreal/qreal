#include "downloader.h"

Downloader::Downloader(QObject *parent)
	: QObject(parent)
	, mReply(NULL)
{
}

void Downloader::getUpdateDetails(QUrl const url)
{
	connect(&mManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(detailsFileDownloaded(QNetworkReply*)));
	sendRequest(url);
}

void Downloader::getUpdate(QUrl const url) throw(CreateFileException)
{
	QString fileName = QFileInfo(url.path()).fileName();
	if (fileName.isEmpty())
		fileName = "update";

	if (QFile::exists(fileName)) {
		QFile::remove(fileName);
	}

	mFile = new QFile(fileName);
	if (!mFile->open(QIODevice::WriteOnly)) {
		delete mFile;
		mFile = NULL;
		throw CreateFileException();
	}

	startFileDownloading(url);
}

void Downloader::detailsFileDownloaded(QNetworkReply *reply)
{
	disconnect(&mManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(detailsFileDownloaded(QNetworkReply*)));
	if (reply->error()) {
		emit detailsLoadError(qPrintable(reply->errorString()));
	} else {
		emit detailsDownloaded(reply);
	}
}

void Downloader::updatesFileDownloaded(QNetworkReply *reply)
{
	disconnect(&mManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(updatesFileDownloaded(QNetworkReply*)));
	if (mReply->error()) {
		emit updatesLoadError(qPrintable(reply->errorString()));
		mFile->remove();
	} else {
		mFile->flush();
		mFile->close();
		emit updatesDownloaded(QFileInfo(*mFile).filePath());
	}

	mReply->deleteLater();
	mReply = 0;
	delete mFile;
}

void Downloader::fileReadyRead()
{
	if (mFile) {
		mFile->write(mReply->readAll());
	}
}

void Downloader::sendRequest(QUrl const url)
{
	QNetworkRequest request(url);
	mReply = mManager.get(request);
}

void Downloader::startFileDownloading(const QUrl url)
{
	connect(&mManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(updatesFileDownloaded(QNetworkReply*)));
	sendRequest(url);
	connect(mReply, SIGNAL(readyRead()), this, SLOT(fileReadyRead()));
}
