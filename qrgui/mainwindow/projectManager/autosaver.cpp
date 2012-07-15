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
	reinit();
}

void Autosaver::reinit()
{
	if (SettingsManager::value("Autosave").toBool()) {
		mTimer->start(interval() * 1000);
	} else {
		mTimer->stop();
	}
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
	if (!mProjectManager->save()) {
		mProjectManager->saveAs(filePath());
	}
}

QString Autosaver::filePath()
{
	QString result;
	if (SettingsManager::value("AutosaveFileName").toString().isEmpty()) {
		result = QApplication::applicationFilePath() + ".qrs";
	} else {
		result = SettingsManager::value("AutosaveDirPath").toString();
		if (result.isEmpty()) {
			result = QApplication::applicationDirPath();
		}
		result += "/" + SettingsManager::value("AutosaveFileName").toString();
	}
	return result;
}
