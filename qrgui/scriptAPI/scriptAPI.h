#pragma once

#include <QtScript/QScriptEngine>
#include <QtCore/QEventLoop>

#include "guiFacade.h"
#include "virtualCursor.h"
#include "virtualKeyboard.h"
#include "hintAPI.h"
#include "sceneAPI.h"
#include "paletteAPI.h"

namespace qReal {

namespace gui {

/// Represents script API for system to interact with graphic user interface. This class provides access to
/// control virtual devices (like classes: VirtualCursor, VirtualKeyboard), API for some elements of graphic
/// user interface (like classes: PaletteAPI, SceneAPI), GUI facades for system and plugins (like classes:
/// GuiFacade, TwoDModelGuiFacade. This classes can search elements of graphics interface, which needed to
/// use API and virtual tools), and class HintAPI, which allows to add hints and navigation arrows to any
/// graphical object. In total, this API interpretate commands on QtScript with access to graphic user
/// interface, which can reproduce some user actions in some order, which would be made with assistance of
/// VirtualCursor and VirtualMouse, like user would made them himself.
class ScriptAPI : public QObject
{
	Q_OBJECT

public:
	ScriptAPI();
	~ScriptAPI();

	/// Initialize script API.
	void init(MainWindow *mainWindow);

	/// Stops execution for duration. Starting event loop, which breaking when duration time ellapsed.
	Q_INVOKABLE void wait(int duration);

	/// Changes virtual cursor parent. Virtual cursor is QWidget child, so his icon must have parent.
	Q_INVOKABLE void changeWindow(QWidget *parent);

	/// Picks combo box item. Virtual cursor can pick any item in sight of view. Cant scroll bars.
	Q_INVOKABLE void pickComboBoxItem(QComboBox *comboBox, QString const &name, int const duration);

	/// Will make plugin gui facade available in scripting file.
	Q_INVOKABLE void loadPluginGuiFacade(QString const pluginName);

	/// Returns gui facade.
	GuiFacade *guiFacade();

	/// Returns virtual cursor.
	VirtualCursor *virtualCursor();

	/// Returns virtual keyboard.
	VirtualKeyboard *virtualKeyboard();

	/// Returns scene API.
	SceneAPI *sceneAPI();

	/// Returns hint API.
	HintAPI *hintAPI();

	/// Returns palette API.
	PaletteAPI *paletteAPI();

public slots:
	///Breaks event loop.
	void breakWaiting();

	///Aborts evaluating of script.
	void abortEvaluate();

private:
	GuiFacade *mGuiFacade;
	VirtualCursor *mVirtualCursor;
	VirtualKeyboard *mVirtualKeyboard;
	SceneAPI *mSceneAPI;
	PaletteAPI *mPaletteAPI;
	HintAPI *mHintAPI;

	MainWindow *mMainWindow;
	QScriptEngine mScriptEngine;
	QEventLoop mEventLoop;
};

}
}
