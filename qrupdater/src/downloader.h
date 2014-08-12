#pragma once

#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtCore/QFileInfo>

namespace qrUpdater {

/// Provides two-ways of downloading: to IODevice and to file.
class Downloader : public QObject
{
	Q_OBJECT
public:
	class CreateFileException{};

	explicit Downloader(QObject *parent = 0);

	/// Downloads lightweight data and emits signal with IODevice.
	void getUpdateDetails(QUrl const &url);

	/// Downloads to file, emits filepath.
	void getUpdateFiles(QList<QUrl> const &urls);

signals:
	void detailsDownloaded(QIODevice *reply);
	void updateDownloaded(QUrl const &url, QString const &filePath);
	void downloadingFinished();
	void detailsLoadError(QString const &error);
	void updatesLoadError(QString const &error);

protected:
	void sendRequest(QUrl const &url);
	void getUpdate(QUrl const &url) throw(CreateFileException);
	void startFileDownloading(QUrl const &url);
	void downloadNext();

	int mLoadedFileIndex;
	QNetworkAccessManager mManager;
	QList<QUrl> mFilesToDownload;
	QNetworkReply *mReply;
	QFile *mFile;

protected slots:
	void detailsFileDownloaded(QNetworkReply *reply);
	void updatesFileDownloaded(QNetworkReply *reply);
	void fileReadyRead();
};
}
