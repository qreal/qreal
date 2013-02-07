#pragma once

#include "sceneCustomizationInterface.h"
#include "editorView.h"
#include "../mainwindow/mainWindow.h"

namespace qReal
{

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
