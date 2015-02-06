#include "autosaver.h"

#include <QtCore/QFileInfo>
#include <QtWidgets/QApplication>

#include <qrkernel/settingsManager.h>

#include "qrgui/systemFacade/components/projectManager.h"

using namespace qReal;

Autosaver::Autosaver(ProjectManager &projectManager)
	: QObject(&projectManager)
	, mProjectManager(projectManager)
{
	connect(&mTimer, &QTimer::timeout, this, &Autosaver::saveAutoSave);
}

void Autosaver::reinit()
{
	resume();
}

void Autosaver::resume()
{
	if (SettingsManager::value("Autosave").toBool()) {
		mTimer.start(interval() * 1000);
	} else {
		mTimer.stop();
	}
}

void Autosaver::suspend()
{
	mTimer.stop();
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
	return autosaveFilePath(mProjectManager.saveFilePath());
}

QString Autosaver::autosaveFilePath(const QString &currentFilePath) const
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

bool Autosaver::isAutosave(const QString &fileName) const
{
	return QFileInfo(fileName).fileName().contains("~");
}

bool Autosaver::isTempFile(const QString &fileName) const
{
	return fileName == tempFilePath();
}

QString Autosaver::originalFile(const QString &fileName) const
{
	if (!isAutosave(fileName)) {
		return fileName;
	}

	QFileInfo fileInfo(fileName);
	return fileInfo.absolutePath() + "/" + fileInfo.fileName().remove("~");
}

bool Autosaver::openTemp()
{
	return mProjectManager.open(tempFilePath());
}

bool Autosaver::openAutosave(const QString &fileName)
{
	return mProjectManager.open(autosaveFilePath(fileName));
}

void Autosaver::saveTemp()
{
	mProjectManager.saveTo(tempFilePath());
}

bool Autosaver::checkAutoSavedVersion(const QString &originalProjectPath)
{
	QString const autosave = autosaveFilePath(originalProjectPath);
	QFileInfo const autosaveInfo(autosave);
	if (!autosaveInfo.exists() || autosave == originalProjectPath) {
		return false;
	}

	if (!mProjectManager.askQuestion(tr("Question"), openAutosavePrompt())) {
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

	if (!mProjectManager.askQuestion(tr("Question"), openTempFilePrompt())) {
		QFile(tempFileInfo.absoluteFilePath()).remove();
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
	mProjectManager.saveTo(autosaveFilePath());
}

bool Autosaver::removeFile(const QString &fileName)
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

bool Autosaver::removeAutoSave(const QString &fileName)
{
	return removeFile(autosaveFilePath(fileName));
}
