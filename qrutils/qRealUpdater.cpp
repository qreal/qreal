#include "qRealUpdater.h"

using namespace utils;

QRealUpdater::QRealUpdater(QString const &applicationPath)
	: mHasNewUpdates(false)
	, mUpdaterProcess(nullptr)
	, mQRealPath(applicationPath)

{
	mUpdaterPath = QFileInfo(mQRealPath).absolutePath() + "/qrUpdater";
}

bool QRealUpdater::hasUpdates()
{
	return mHasNewUpdates;
}

void QRealUpdater::startUpdater()
{
	if (!hasUpdatePermission()) {
		mHasNewUpdates = false;
		return;
	}
	executeUpdater();
	transferInfo();
}

void QRealUpdater::executeUpdater()
{
	QString const programPath = mUpdaterPath + "/qrUpdater";

	mUpdaterProcess = new QProcess();
	mUpdaterProcess->setWorkingDirectory(mUpdaterPath);

	mUpdaterProcess->start(programPath, collectArguments());
}

void QRealUpdater::transferInfo()
{
	mUpdaterProcess->write(QString(mQRealPath + "\n").toUtf8());
	mUpdaterProcess->waitForReadyRead(updaterTimeout);
	readAnswer();
}

QStringList QRealUpdater::collectArguments()
{
	QStringList arguments;
	QStringList followingUnits;
	followingUnits << "windows" << "windows-qru::self";
	arguments << "-unit" << qReal::SettingsManager::value("updaterFollowUnits", followingUnits).toStringList()
			<< "-version" << qReal::SettingsManager::value("version").toString()
			<< "-url" << qReal::SettingsManager::value("updaterDetailsURL", "http://localhost/updates.xml").toString();
	return arguments;
}

bool QRealUpdater::hasUpdatePermission()
{
	qReal::SettingsManager::instance()->load();
	return !qReal::SettingsManager::value("version", "").toString().isEmpty()
			&& qReal::SettingsManager::value("updaterActive", true).toBool();
}

void QRealUpdater::prepareForClose()
{
	qReal::SettingsManager::setValue("version", "");  // 'couz I cant just get version from inside
	qReal::SettingsManager::instance()->saveData();
}

void QRealUpdater::readAnswer()
{
	QString input(mUpdaterProcess->readAll());
	mHasNewUpdates = input.contains("Terminate!");
	if (mHasNewUpdates) {
		prepareForClose();
	}
}
