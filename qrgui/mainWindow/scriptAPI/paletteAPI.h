/* Copyright 2014-2015 QReal Research Group, Dmitry Chernov, Dmitry Mordvinov
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

#include <QtCore/QObject>

namespace qReal {

class MainWindow;

namespace gui {

class ScriptAPI;

/// Provides scripting API for working with QReal palette. Uses virtual mouse primitives to complete them.
class PaletteAPI : public QObject
{
	Q_OBJECT

public:
	PaletteAPI(ScriptAPI &scriptAPI, MainWindow &mainWindow);

	/// Drags palette element to scene with virtual cursor for \a duration. Virtual cursor moving to
	/// palette, redrawing with icon of palette element, then moving to scene. Creates block on scene
	/// in the given coordinates.
	/// @param paletteElementId A string representation of metatype of the element dragged out.
	Q_INVOKABLE QString dragPaletteElement(const QString &paletteElementId, int duration
			, int xSceneCoord, int ySceneCoord);

private:
	ScriptAPI &mScriptAPI;
	MainWindow &mMainWindow;
};

}
}

