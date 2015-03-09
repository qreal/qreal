#include "updater.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QFileInfo>
#include <QtCore/QProcess>
#include <QtXml/QDomDocument>

#include <qrkernel/logging.h>

using namespace updatesChecker;

Updater::Updater(QObject *parent)
	: QObject(parent)
	, mUpdaterProcess(nullptr)
{
}

void Updater::checkForNewVersion()
{
	executeUpdater("--checkupdates");
}

void Updater::start()
{
	executeUpdater("--updater");
}

void Updater::executeUpdater(const QString &mode)
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

void Updater::readAnswer()
{
	const QString output = mUpdaterProcess->readAllStandardOutput();
	// Checking that output is a valid XML
	QDomDocument parser;
	parser.setContent(output);
	QLOG_INFO() << "Updater output:" << output;
	if (!output.isEmpty() && !parser.isNull() && output.trimmed().startsWith("<")) {
		emit newVersionAvailable();
	} else {
		emit noNewVersionAvailable();
	}
}
