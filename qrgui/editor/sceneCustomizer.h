#pragma once

#include "editor/sceneCustomizationInterface.h"
#include "editor/editorView.h"
#include "mainWindow/mainWindow.h"

namespace qReal {

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
