#include <QtGui/QApplication>

#include "../../qrkernel/settingsManager.h"
#include "projectManager.h"
#include "autosaver.h"

using namespace qReal;

Autosaver::Autosaver(ProjectManager *projectManager)
	: QObject(projectManager)
	, mProjectManager(projectManager)
	, mTimer(new QTimer(this))
{
	connect(mTimer, SIGNAL(timeout()), this, SLOT(save()));
}

void Autosaver::reinit()
{
	if (SettingsManager::value("Autosave").toBool()) {
		mTimer->start(interval() * 1000);
	} else {
		mTimer->stop();
	}
}

void Autosaver::stop()
{
	mTimer->stop();
}

uint Autosaver::interval()
{
	uint result = SettingsManager::value("AutosaveInterval").toUInt();
	if (result == 0) {
		result = defaultInterval;
	}
	return result;
}

void Autosaver::save()
{
	mProjectManager->save();
}

QString Autosaver::filePath()
{
	QString result;
	if (SettingsManager::value("AutosaveFileName").toString().isEmpty()) {
		return QApplication::applicationFilePath() + ".qrs";
	}

	result = SettingsManager::value("AutosaveDirPath").toString();
	if (result.isEmpty()) {
		result = QApplication::applicationDirPath();
	}
	result += "/" + SettingsManager::value("AutosaveFileName").toString();
	return result;
}

Autosaver::Pauser::Pauser(Autosaver *autosaver)
		:mAutosaver(autosaver)
{
	autosaver->stop();
}

Autosaver::Pauser::~Pauser()
{
	mAutosaver->reinit();
}

Autosaver::Pauser Autosaver::pauser()
{
	return Pauser(this);
}
