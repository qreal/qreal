/* Copyright 2014-2016 QReal Research Group, Dmitry Chernov, Dmitry Mordvinov, CyberTech Labs Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#pragma once

#include <QtScript/QScriptEngine>
#include <QtCore/QEventLoop>
#include <QtScript/QScriptEngineAgent>

class QComboBox;
class QAbstractScrollArea;

namespace qReal {

class MainWindow;

namespace gui {

class GuiFacade;
class VirtualCursor;
class VirtualKeyboard;
class SceneAPI;
class PaletteAPI;
class HintAPI;
class Utils;

/// Represents scripting API for interaction with QReal graphical user interface. This class provides access to
/// controling virtual devices (see VirtualCursor, VirtualKeyboard), API for some elements of graphical
/// user interface (see PaletteAPI, SceneAPI), GUI facades for system and plugins (see GuiFacade,
/// ToolPluginManager::pluginGuiFacade -- this classes can search elements of graphical interface which are needed
/// for API and virtual tools) and class HintAPI which allows to add hints and navigation arrows to any graphical
/// object. All together this API provides commands for QtScript for accessing the graphic user interface and
/// reproducing some user actions in some order like user will do it himself.
/// @warning Should not throw C++ except-s in q_invokable methods, a script engine can't propagate ones up the stack.
/// All asserts for such functions, for passed parameters should be done in a script, using script's assert().
/// This applies to all invokable functions of the scriptAPI module, functions are invokable from ui(), utils(), etc.
class ScriptAPI : public QObject
{
	Q_OBJECT

public:
	static const int processEventsInterval = 20;

	ScriptAPI();
	~ScriptAPI();

	/// Initializes scripting API.
	void init(MainWindow &mainWindow);

	/// Starts evaluatiion of the script with script engine.
	void evaluate();

	/// Starts evaluatiion of the script "script" with script engine.
	void evaluateScript(const QString &script, const QString &fileName);

	/// Starts evaluatiion of the script with script engine located in "fileName".
	void evaluateFileScript(const QString &fileName);

	/// Registres new function fun in QScriptEngine for using fun in scripts.
	/// @param fun must be a C++ function with signature QScriptEngine::FunctionSignature.
	/// @param length is the number of arguments that \a fun expects.
	void registerNewFunction(QScriptEngine::FunctionSignature fun, const QString &qScriptName, int length = 0);

	/// Checks the syntax of the given script.
	QScriptSyntaxCheckResult checkSyntax(const QString &script) const;

	/// Checks an existence of uncaught exception.
	bool hasUncaughtException() const;

	/// Clears any uncaught exceptions in corresponding engine.
	void clearExceptions();

	/// Returns a human-readable backtrace of the last uncaught exception.
	QStringList uncaughtExceptionBacktrace() const;

	/// Returns the current uncaught exception, or an invalid QScriptValue if there is no uncaught exception.
	/// The exception value is typically an Error object; in that case
	/// you can call toString() on the return value to obtain an error message.
	QScriptValue uncaughtException() const;

	/// Returns internal Qt scripting engine object used for invoking Qt Script code.
	QScriptEngine *engine();

	/// Freezes execution for duration. Starting event loop breaking when duration time ellapsed.
	Q_INVOKABLE void wait(int duration) noexcept;

	/// Attaches virtual cursor to a new top-level window.
	/// @todo: move it to VirtualCursor.
	Q_INVOKABLE void switchMouseCursorToWindow(QWidget *parent) noexcept;

	/// Attaches virtual cursor to a mainWindow.
	/// @todo: move it to VirtualCursor.
	Q_INVOKABLE void switchMouseCursorToMainWindow() noexcept;

	/// Picks combo box item. Virtual cursor can pick any item in its sight of view.
	/// Temporary doesn`t work with scroll bars.
	/// @todo: move it to Utils as a action.
	Q_INVOKABLE void pickComboBoxItem(QComboBox *comboBox, const QString &name, int duration) noexcept;

	/// Changes QAbstractScrollArea viewport to make target widget visible. Can work only with vertical sliders.
	/// @todo: move it to Utils as a action.
	Q_INVOKABLE void scroll(QAbstractScrollArea *area, QWidget *widget, int duration) noexcept;

	/// Provides operations for obtaining different UI parts of some tool plugin.
	Q_INVOKABLE QScriptValue pluginUi(const QString &pluginName);

	/// Povides operations for obtaining different UI parts.
	Q_INVOKABLE QScriptValue ui();

	/// Provides operations for managing mouse simulator.
	Q_INVOKABLE QScriptValue cursor();

	/// Provides operations for managing keyboard simulator.
	Q_INVOKABLE QScriptValue keyboard();

	/// Provides extended operations with QReal scene.
	Q_INVOKABLE QScriptValue scene();

	/// Provides extended operations for pointing out some options to user.
	Q_INVOKABLE QScriptValue hints();

	/// Provides extended operations with QReal palette.
	Q_INVOKABLE QScriptValue palette();

	/// Provides some extended API for interaction with QReal logic and UI.
	Q_INVOKABLE QScriptValue utils();

	/// Returns GUI facade for obtaining different UI parts.
	GuiFacade &guiFacade();

	/// Returns virtual cursor for managing mouse cursor simulator.
	VirtualCursor &virtualCursor();

	/// Returns an instance of SceneAPI for extended work with scene.
	SceneAPI &sceneAPI();

public slots:
	/// Breaks passive event loop.
	void breakWaiting();

	/// Aborts evaluation of the script.
	void abortEvaluation();

private:
	QScopedPointer<GuiFacade> mGuiFacade;
	QScopedPointer<VirtualCursor> mVirtualCursor;
	QScopedPointer<VirtualKeyboard> mVirtualKeyboard;
	QScopedPointer<SceneAPI> mSceneAPI;
	QScopedPointer<PaletteAPI> mPaletteAPI;
	QScopedPointer<HintAPI> mHintAPI;
	QScopedPointer<Utils> mUtilsApi;

	QScriptEngine mScriptEngine;
	QEventLoop mEventLoop;
};

}
}
