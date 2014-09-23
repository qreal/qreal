#pragma once

#include <QtWidgets/QAction>

#include <qrgui/editor/sceneCustomizationInterface.h>

namespace interpreterCore {

/// Updates title visibilities on scene when corresponding setting changes, stores setting value in registry.
/// Depends on "showTitlesForRobots" registry setting.
class TitlesVisibilityManager : public QObject
{
	Q_OBJECT

public:
	/// Constructor.
	/// @param titlesVisibilityAction - QAction corresponding to titles visibility setting.
	/// @param sceneCustomizer - has methods to control titles visibility on scene.
	TitlesVisibilityManager(QAction &titlesVisibilityAction, qReal::SceneCustomizationInterface &sceneCustomizer);

private slots:
	void onTitlesVisibilityChanged(bool checked);

private:
	QAction &mTitlesVisibilityAction;
	qReal::SceneCustomizationInterface &mSceneCustomizer;
};

}
