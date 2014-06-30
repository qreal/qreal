#include "qRealUpdater.h"

using namespace qReal;

QRealUpdater::QRealUpdater(char const *applicationPath)
	: QObject()
	, mHasNewUpdates(false)
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
	arguments << "-unit" << SettingsManager::value("updaterFollowUnits", followingUnits).toStringList()
			<< "-version" << SettingsManager::value("version").toString()
			<< "-url" << SettingsManager::value("updaterDetailsURL", "http://localhost/updates.xml").toString();
	return arguments;
}

bool QRealUpdater::hasUpdatePermission()
{
	SettingsManager::instance()->load();
	return !SettingsManager::value("version", "").toString().isEmpty()
			&& SettingsManager::value("updaterActive", true).toBool();
}

void QRealUpdater::prepareForClose()
{
	SettingsManager::setValue("version", "");  // 'couz I cant just get version from inside
	SettingsManager::instance()->saveData();
}

void QRealUpdater::readAnswer()
{
	QString input(mUpdaterProcess->readAll());
	mHasNewUpdates = input.contains("Terminate!");
	if (mHasNewUpdates) {
		prepareForClose();
	}
}
