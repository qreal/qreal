#include "update.h"

#include <qrkernel/logging.h>

namespace markers {
static QString const selfInstalling = "qru::self";
}

using namespace qrUpdater;

Update::Update(QObject *parent)
	: QObject(parent)
	, mIsInstalled(false)
	, mProcess(nullptr)
{
}

Update::Update(QString const &filePath, QStringList const &args, QString const &version, QObject *parent)
	: QObject(parent)
	, mIsInstalled(false)
	, mProcess(nullptr)
{
	setData(filePath, args, version);
}

void Update::setUrl(QUrl const &link)
{
	mDownloadUrl = link;
}

void Update::setUnitName(QString const &unit)
{
	mModule = unit;
}

void Update::setFilePath(QString const &path)
{
	mFilePath = path;
}

void Update::setData(QString const &filePath, QStringList const &args, QString const &version, QUrl const &link)
{
	mFilePath = filePath;
	mArguments = args;
	mVersion = qReal::Version::fromString(version);
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
	mProcess = nullptr;
	mIsInstalled = false;
}

#ifdef Q_OS_WIN
#include <windows.h>
#include <shellapi.h>
#endif

void Update::installUpdate()
{
#ifdef Q_OS_WIN
	/// @todo: On windows we cannot start installer with QProcess class
	/// because it cannot call UAC screen. So running it like detached process.
	mModule += markers::selfInstalling;

	char const *name = qPrintable(QFileInfo(mFilePath).absoluteFilePath());
	char const *args = qPrintable(mArguments.join(" "));
	int const result = (int)::ShellExecuteA(0, 0, name, args, 0, SW_SHOWNORMAL);
//	if (SE_ERR_ACCESSDENIED == result) {
//		// Requesting elevation
//		result = (int)::ShellExecuteA(0, "runas", name, args, 0, SW_SHOWNORMAL);
//	}

	if (result <= 32) {
		/// @todo: process error here...
	}
#else
	mProcess = new QProcess(this);
	connect(mProcess, SIGNAL(finished(int,QProcess::ExitStatus))
			, this, SLOT(installationFinished(int,QProcess::ExitStatus)));
	connect(mProcess, static_cast<void (QProcess::*)(QProcess::ProcessError)>(&QProcess::error)
			, [this](QProcess::ProcessError error) {
				QLOG_ERROR() << mProcess->errorString();
				if (error == QProcess::FailedToStart) {
					/// @todo: Process error correctly
					// This hack will terminate program
					mModule += markers::selfInstalling;
				}
	});

	if (!hasSelfInstallMarker()) {
		mProcess->start(mFilePath, mArguments);
	} else {
		mProcess->startDetached(mFilePath, mArguments);
	}
#endif
}

bool Update::isEmpty() const
{
	return mFilePath.isEmpty() || !QFile::exists(mFilePath);
}

bool Update::isInstalling() const
{
	return mProcess && (mProcess->state() != QProcess::NotRunning);
}

bool Update::isInstalled() const
{
	return mIsInstalled;
}

bool Update::hasSelfInstallMarker() const
{
	return unit().contains(markers::selfInstalling);
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

qReal::Version Update::version() const
{
	return mVersion;
}

QStringList &Update::arguments()
{
	return mArguments;
}

void Update::installationFinished(int exitCode, QProcess::ExitStatus status)
{
	Q_UNUSED(exitCode);
	mIsInstalled = true;
	emit installed(status == QProcess::NormalExit);
}
