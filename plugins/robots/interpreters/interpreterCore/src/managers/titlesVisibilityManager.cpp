#include "titlesVisibilityManager.h"

#include <qrkernel/settingsManager.h>

using namespace interpreterCore;

TitlesVisibilityManager::TitlesVisibilityManager(
		QAction &titlesVisibilityAction
		, qReal::SceneCustomizationInterface &sceneCustomizer)
	: mTitlesVisibilityAction(titlesVisibilityAction)
	, mSceneCustomizer(sceneCustomizer)
{
}

void TitlesVisibilityManager::loadTitlesVisibility()
{
	bool const titlesVisible = qReal::SettingsManager::value("showTitlesForRobots").toBool();
	onTitlesVisibilityChanged(titlesVisible);
}

void TitlesVisibilityManager::onTitlesVisibilityChanged(bool checked)
{
	if (mTitlesVisibilityAction.isChecked() != checked) {
		mTitlesVisibilityAction.setChecked(checked);
		qReal::SettingsManager::setValue("showTitlesForRobots", checked);
		mSceneCustomizer.setTitlesVisible(checked);
		emit titlesVisibilityChanged(checked);
	}
}
