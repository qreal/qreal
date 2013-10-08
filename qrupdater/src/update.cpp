#include "update.h"

Update::Update(QObject *parent)
	: QObject(parent)
	, mIsInstalled(false)
	, mProcess(NULL)
{
}

Update::Update(QString const filePath, QStringList const args, QString const version, QObject *parent)
	: QObject(parent)
	, mIsInstalled(false)
	, mProcess(NULL)
{
	setData(filePath, args, version);
}

void Update::setUrl(const QUrl link)
{
	mDownloadUrl = link;
}

void Update::setUnitName(const QString unit)
{
	mModule = unit;
}

void Update::setData(const QString filePath, const QStringList args, const QString version, const QUrl link)
{
	mFilePath = filePath;
	mArguments = args;
	mVersion = version;
	mDownloadUrl = link;
	mIsInstalled = false;
}

void Update::clear()
{
	if (QFile::exists(mFilePath)) {
		QFile::remove(mFilePath);
	}
	setData(QString(), QStringList(), QString());
	setUnitName("");
	if (isInstalling()) {
		mProcess->terminate();
	}
	mProcess->deleteLater();
	mProcess = NULL;
	mIsInstalled = false;
}

void Update::installUpdate()
{
	mProcess = new QProcess(this);
	connect(mProcess, SIGNAL(finished(int, QProcess::ExitStatus))
			, this , SLOT(installingFinished(int, QProcess::ExitStatus)));
	mProcess->start(mFilePath, mArguments);
}

bool Update::isEmpty() const
{
	return mFilePath.isEmpty() || !QFile::exists(mFilePath);
}

bool Update::isInstalling() const
{
	return (mProcess != NULL) && (mProcess->state() != QProcess::NotRunning);
}

bool Update::isInstalled() const
{
	return mIsInstalled;
}

QUrl Update::url() const
{
	return mDownloadUrl;
}

QString Update::unit() const
{
	return mModule;
}

QString Update::filePath() const
{
	return mFilePath;
}

QString Update::fileName() const
{
	return QFileInfo(mFilePath).fileName();
}

QString Update::version() const
{
	return mVersion;
}

QStringList Update::arguments() const
{
	return mArguments;
}

void Update::installingFinished(int exitCode, QProcess::ExitStatus status)
{
	mIsInstalled = true;
	emit installFinished(status == QProcess::NormalExit);
	Q_UNUSED(exitCode);
}
