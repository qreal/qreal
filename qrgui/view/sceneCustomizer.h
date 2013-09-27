#pragma once

#include "view/sceneCustomizationInterface.h"
#include "view/editorView.h"
#include "mainwindow/mainWindow.h"

namespace qReal
{

/// Implements some QReal scene customizations
class SceneCustomizer : public SceneCustomizationInterface
{
public:
	explicit SceneCustomizer(MainWindow *mainWindow);
	virtual ~SceneCustomizer() {}

	/// Customizes given view with accumulated settings
	void customizeView(EditorView *view);

	void setTitlesVisible(bool visible);

private:
	void refreshCustomization();

	MainWindow *mMainWindow;
	bool mTitlesVisible;
};

}
