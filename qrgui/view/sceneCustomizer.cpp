#include "sceneCustomizer.h"

using namespace qReal;

SceneCustomizer::SceneCustomizer(MainWindow *mainWindow)
	: mMainWindow(mainWindow)
	, mTitlesVisible(true)
{
}

void SceneCustomizer::customizeView(EditorView *view)
{
	view->setTitlesVisible(mTitlesVisible);
}

void SceneCustomizer::refreshCustomization()
{
	QListIterator<EditorView *> views = mMainWindow->openedEditorViews();
	while (views.hasNext()) {
		customizeView(views.next());
	}
}

void SceneCustomizer::setTitlesVisible(bool visible)
{
	mTitlesVisible = visible;
	refreshCustomization();
}
