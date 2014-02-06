#include "titlesVisibilityManager.h"

#include <qrkernel/settingsManager.h>

using namespace interpreterCore;

TitlesVisibilityManager::TitlesVisibilityManager(
		QAction &titlesVisibilityAction
		, qReal::SceneCustomizationInterface &sceneCustomizer)
	: mTitlesVisibilityAction(titlesVisibilityAction)
	, mSceneCustomizer(sceneCustomizer)
{
	bool const titlesVisible = qReal::SettingsManager::value("showTitlesForRobots").toBool();
	onTitlesVisibilityChanged(titlesVisible);

	QObject::connect(
			&mTitlesVisibilityAction
			, &QAction::triggered
			, this
			, &TitlesVisibilityManager::onTitlesVisibilityChanged
			);
}

void TitlesVisibilityManager::onTitlesVisibilityChanged(bool checked)
{
	qReal::SettingsManager::setValue("showTitlesForRobots", checked);
	mSceneCustomizer.setTitlesVisible(checked);
}
