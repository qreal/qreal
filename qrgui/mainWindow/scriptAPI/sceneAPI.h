#pragma once

#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QObject>
#include <QtCore/QVector>

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

	/// Draws link on scene between element with fromElementId and toElementId.
	Q_INVOKABLE void drawLink(QString const &fromElementId, QString const &toElementId, int const duration);

	/// Returns list of nodes id from scene by diagram and element names.
	Q_INVOKABLE QStringList nodeList(QString const &diagram, QString const &element);

	/// Creates block with id, like paletteElement on scene on (xSceneCoord, ySceneCoord) point on scene.
	QString createBlockOnScene(DraggableElement const  *paletteElement, int const xSceneCoord, int const ySceneCoord);

private:
	ScriptAPI *mScriptAPI;
	MainWindow *mMainWindow;
};

}
}
