#include "qRealUpdater.h"

#include <QtCore/QFileInfo>

#include <qrkernel/platformInfo.h>

using namespace utils;

QString const updateServerUrl = "http://127.0.0.1/updates.xml";

QRealUpdater::QRealUpdater(QString const &applicationPath
		, QString const &unit
		, qReal::Version const &version
		, QObject *parent)
	: QObject(parent)
	, mQRealPath(applicationPath)
	, mUpdaterPath(QFileInfo(mQRealPath).absolutePath() + "/qrupdater")
	, mUnit(QString("%1-%2").arg(unit, qReal::PlatformInfo::os()))
	, mVersion(version)
	, mUpdaterProcess(nullptr)
{
}

void QRealUpdater::checkForNewVersion(bool downloadIfFound)
{
	mMustBeDownloadedAfterCheck = downloadIfFound;
	executeUpdater(downloadIfFound ? "--download" : "--check");
}

void QRealUpdater::installUpdates()
{
	mMustBeDownloadedAfterCheck = false;
	executeUpdater("--install");
}

void QRealUpdater::downloadAndInstall()
{
	mMustBeDownloadedAfterCheck = false;
	executeUpdater("--downloadAndInstall");
}

void QRealUpdater::executeUpdater(QString const &mode)
{
	mUpdaterProcess = new QProcess(this);
	mUpdaterProcess->setWorkingDirectory(mUpdaterPath);
	connect(mUpdaterProcess, SIGNAL(finished(int)), this, SLOT(readAnswer()));

	mUpdaterProcess->start(mUpdaterPath, arguments(mode));
}

QStringList QRealUpdater::arguments(QString const &mode)
{
	return { mode
			, "-unit", mUnit, mUnit + "-qru::self"
			, "-version", mVersion.toString()
			/// @todo: Make it customizable!
			, "-url", updateServerUrl
			, "-path", mQRealPath
	};
}

void QRealUpdater::readAnswer()
{
	QString const output = mUpdaterProcess->readAllStandardOutput();
	qDebug() << output;
	QString const marker = mMustBeDownloadedAfterCheck ? "Downloaded" : "Found";
	QRegExp const versionsMatcher(QString(".*%1 .* of version (.*)\\!.*").arg(marker));
	qDebug() << versionsMatcher.pattern();
	if (versionsMatcher.exactMatch(output)) {
		// There may be many messages about the new version, but we ignore it and using the first.
		QString const newVersion = versionsMatcher.cap(1);
		qDebug() << newVersion;
		emit newVersionAvailable(qReal::Version::fromString(newVersion));
	}
}
