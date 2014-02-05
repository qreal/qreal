#pragma once

#include <QtWidgets/QAction>

#include <qrgui/view/sceneCustomizationInterface.h>

namespace interpreterCore {

/// \todo Comment
/// Depends on "showTitlesForRobots" registry setting.
class TitlesVisibilityManager : public QObject
{
	Q_OBJECT

public:
	TitlesVisibilityManager(QAction &titlesVisibilityAction, qReal::SceneCustomizationInterface &sceneCustomizer);

	void loadTitlesVisibility();

public slots:
	void onTitlesVisibilityChanged(bool checked);

signals:
	void titlesVisibilityChanged(bool checked);

private:
	QAction &mTitlesVisibilityAction;
	qReal::SceneCustomizationInterface &mSceneCustomizer;
};

}
