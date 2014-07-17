#pragma once

#include <QtCore/QString>
#include <QtGui/QIcon>

namespace qReal {

class MainWindow;

namespace gui {

class ScriptAPI;

/// This class provides script API for some actions which use palette. Uses virtual mouse primitives to
/// complete them.
class PaletteAPI  : public QObject
{
	Q_OBJECT

public:
	PaletteAPI(ScriptAPI *mScriptAPI, MainWindow *mainWindow);

	/// Drag palette element to scene with virtual cursor by id for duration. Virtual cursor moving to
	/// palette, redrawing with icon of palette element, then moving to scene. Creates block on scene by
	/// coordinates.
	Q_INVOKABLE QString dragPaletteElement(QString const &paletteElementId, int const duration, int const xSceneCoord, int const ySceneCoord);

private:
	ScriptAPI *mScriptAPI;
	MainWindow *mMainWindow;
};

}
}

