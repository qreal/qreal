#pragma once

#include <QtCore/QString>
#include <QtCore/QObject>

namespace qReal {

class MainWindow;

namespace gui {

class DraggableElement;
class ScriptAPI;

/// This class provides script API for some actions on scene. Use virtual mouse primitives to complete them.

class SceneAPI  : public QObject
{
	Q_OBJECT

public:
	SceneAPI(ScriptAPI *mScriptAPI, MainWindow *mainWindow);

	/// Returns Id of start node.
	Q_INVOKABLE QString initialNode();

	/// Draw link on scene between element with fromElementId and toElementId.
	Q_INVOKABLE void drawLink(QString const &fromElementId, QString const &toElementId, int const duration);

	/// Create block with id, like paletteElement on scene on (xSceneCoord, ySceneCoord) point on scene.
	QString createBlockOnScene(DraggableElement const  *paletteElement, int const xSceneCoord, int const ySceneCoord);

private:
	ScriptAPI *mScriptAPI;
	MainWindow *mMainWindow;
};

}
}
