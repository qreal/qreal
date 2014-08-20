#include "qRealUpdater.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QFileInfo>
#include <QtCore/QProcess>
#include <QtXml/QDomDocument>

using namespace utils;

QRealUpdater::QRealUpdater(QObject *parent)
	: QObject(parent)
	, mUpdaterProcess(nullptr)
{
}

void QRealUpdater::checkForNewVersion()
{
	executeUpdater("--checkupdates");
}

void QRealUpdater::start()
{
	executeUpdater("--updater");
}

void QRealUpdater::executeUpdater(QString const &mode)
{
	mUpdaterProcess = new QProcess(this);
	mUpdaterProcess->setWorkingDirectory(QCoreApplication::applicationDirPath());
	connect(mUpdaterProcess, SIGNAL(finished(int)), this, SLOT(readAnswer()));

	if (mode == "--updater") {
		mUpdaterProcess->startDetached(QCoreApplication::applicationDirPath() + "/maintenance", {mode});
	} else {
		mUpdaterProcess->start(QCoreApplication::applicationDirPath() + "/maintenance", {mode});
	}
}

void QRealUpdater::readAnswer()
{
	QString const output = mUpdaterProcess->readAllStandardOutput();
	// Checking that output is a valid XML
	QDomDocument parser;
	parser.setContent(output);
	if (!parser.isNull()) {
		emit newVersionAvailable();
	}
}
