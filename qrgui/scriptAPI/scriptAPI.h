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

	///Initialize script API.
	void init(MainWindow *mainWindow);

	///Stop execution for duration. Starting event loop, which breaking when duration time ellapsed.
	Q_INVOKABLE void wait(int duration);

	///Change virtual cursor parent. Virtual cursor is QWidget child, so his icon must have parent.
	Q_INVOKABLE void changeWindow(QWidget *parent);

	///Pick combo box item. Virtual cursor can pick any item in sight of view. Cant scroll bars.
	Q_INVOKABLE void pickComboBoxItem(QComboBox *comboBox, QString const &name, int const duration);

	///Returns gui facade.
	Q_INVOKABLE GuiFacade *guiFacade();

	///Returns virtual cursor.
	Q_INVOKABLE VirtualCursor *virtualCursor();

	///Returns virtual keyboard.
	Q_INVOKABLE VirtualKeyboard *virtualKeyboard();

	///Returns scene API.
	Q_INVOKABLE SceneAPI *sceneAPI();

	///Returns hint API.
	Q_INVOKABLE HintAPI *hintAPI();

	///Returns palette API.
	Q_INVOKABLE PaletteAPI *paletteAPI();

public slots:
	///Break event loop.
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
