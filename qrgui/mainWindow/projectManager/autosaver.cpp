#include <QtCore/QFileInfo>
#include <QtCore/QDir>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMessageBox>

#include <qrkernel/settingsManager.h>

#include "mainWindow/projectManager/projectManager.h"
#include "mainWindow/projectManager/autosaver.h"

using namespace qReal;

Autosaver::Autosaver(ProjectManager *projectManager)
	: QObject(projectManager)
	, mProjectManager(projectManager)
	, mTimer(new QTimer(this))
{
	connect(mTimer, SIGNAL(timeout()), this, SLOT(saveAutoSave()));
}

void Autosaver::reinit()
{
	resume();
}

void Autosaver::resume()
{
	if (SettingsManager::value("Autosave").toBool()) {
		mTimer->start(interval() * 1000);
	} else {
		mTimer->stop();
	}
}

void Autosaver::suspend()
{
	mTimer->stop();
}

uint Autosaver::interval() const
{
	uint result = SettingsManager::value("AutosaveInterval").toUInt();
	if (result == 0) {
		result = defaultInterval;
	}
	return result;
}

Autosaver::Pauser::Pauser(Autosaver &autosaver)
		: mAutosaver(autosaver)
{
	autosaver.suspend();
}

Autosaver::Pauser::~Pauser()
{
	mAutosaver.resume();
}

QString Autosaver::autosaveFilePath() const
{
	return autosaveFilePath(mProjectManager->saveFilePath());
}

QString Autosaver::autosaveFilePath(QString const &currentFilePath) const
{
	if (currentFilePath.isEmpty() || currentFilePath == tempFilePath()) {
		return tempFilePath();
	}
	QFileInfo const currentProject(currentFilePath);
	QString const autosaveDirectory = currentProject.absoluteDir().exists()
			? currentProject.absolutePath()
			: QApplication::applicationDirPath();
	QString const currentFileName = currentProject.fileName();
	QString const autosaveFile = currentFileName.startsWith("~")
			? currentFileName : "~" + currentFileName;
	return autosaveDirectory + "/" + autosaveFile;
}

QString Autosaver::tempFilePath() const
{
	return QString("%1/%2.qrs").arg(
			QApplication::applicationDirPath()
			, SettingsManager::value("AutosaveTempFile").toString());
}

bool Autosaver::isAutosave(QString const &fileName) const
{
	return QFileInfo(fileName).fileName().contains("~");
}

bool Autosaver::isTempFile(QString const &fileName) const
{
	return fileName == tempFilePath();
}

QString Autosaver::originalFile(QString const &fileName) const
{
	if (!isAutosave(fileName)) {
		return fileName;
	}
	QFileInfo fileInfo(fileName);
	return fileInfo.absolutePath() + "/" + fileInfo.fileName().remove("~");
}

bool Autosaver::openTemp()
{
	return mProjectManager->open(tempFilePath());
}

bool Autosaver::openAutosave(QString const &fileName)
{
	return mProjectManager->open(autosaveFilePath(fileName));
}

void Autosaver::saveTemp()
{
	mProjectManager->saveTo(tempFilePath());
}

bool Autosaver::checkAutoSavedVersion(QString const &originalProjectPath)
{
	QString const autosave = autosaveFilePath(originalProjectPath);
	QFileInfo const autosaveInfo(autosave);
	if (!autosaveInfo.exists() || autosave == originalProjectPath) {
		return false;
	}
	if (QMessageBox::question(QApplication::activeWindow(), tr("Question")
			, openAutosavePrompt()) != QMessageBox::Yes) {
		return false;
	}
	return openAutosave(originalProjectPath);
}

bool Autosaver::checkTempFile()
{
	QFileInfo const tempFileInfo(tempFilePath());
	if (!tempFileInfo.exists()) {
		return false;
	}
	if (QMessageBox::question(QApplication::activeWindow(), tr("Question")
			, openTempFilePrompt()) != QMessageBox::Yes) {
		return false;
	}
	return openTemp();
}

QString Autosaver::openAutosavePrompt() const
{
	return tr("More recent autosaved version of this file was found. "\
			"Do you wish to open it instead?");
}

QString Autosaver::openTempFilePrompt() const
{
	return tr("It seems like the last application session was terminated in an "\
			"unusial way. Do you wish to recover unsaved project?");
}

void Autosaver::saveAutoSave()
{
	mProjectManager->saveTo(autosaveFilePath());
}

bool Autosaver::removeFile(QString const &fileName)
{
	return QFile::remove(fileName);
}

bool Autosaver::removeTemp()
{
	return removeFile(tempFilePath());
}

bool Autosaver::removeAutoSave()
{
	return removeFile(autosaveFilePath());
}

bool Autosaver::removeAutoSave(QString const &fileName)
{
	return removeFile(autosaveFilePath(fileName));
}
