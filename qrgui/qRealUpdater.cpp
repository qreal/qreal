#include "qRealUpdater.h"

using namespace qReal;

QRealUpdater::QRealUpdater(char *argv[])
	: QObject()
	, mHasAnswer(false)
	, mHasNewUpdates(false)
	, mUpdaterProcess(NULL)
	, mQRealPath(argv[0])

{
	mUpdaterPath = QFileInfo(mQRealPath).absolutePath() + "/QRealUpdater/";
	SettingsManager::instance()->load();
}

bool QRealUpdater::hasUpdates()
{

	while (!mHasAnswer) {
		QCoreApplication::processEvents();
	}
	return mHasNewUpdates;
}

void QRealUpdater::startUpdater()
{
	executeUpdater();
}

void QRealUpdater::executeUpdater()
{
	QString const programPath = mUpdaterPath + "updater";
	QStringList arguments;
	arguments << "-unit" << "windows"
			<< "-version" << SettingsManager::value("version").toString()
			<< "-url" << "http://localhost/updates.xml";

	mUpdaterProcess = new QProcess();
	mUpdaterProcess->setWorkingDirectory(mUpdaterPath);

	connect(mUpdaterProcess, SIGNAL(readyRead()), this, SLOT(readAnswer()));

	mUpdaterProcess->start(programPath, arguments);

	mUpdaterProcess->write(QString(mQRealPath + "\n").toUtf8());
}

void QRealUpdater::readAnswer()
{
	QString input(mUpdaterProcess->readAll());
	mHasNewUpdates = input.contains("Terminate!");
	mHasAnswer = true;
}
