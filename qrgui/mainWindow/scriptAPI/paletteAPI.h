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

