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

/// Provides scripting API for some actions on QReal scene. Uses virtual mouse primitives to complete them.
class SceneAPI : public QObject
{
	Q_OBJECT

public:
	SceneAPI(ScriptAPI &scriptAPI, MainWindow &mainWindow);

	/// Draws link on scene between element with fromElementId and toElementId with mouse gesture.
	/// @param fromElementId The string representation of id of the source element.
	/// @param toElementId The string representation of id of the target element.
	/// @param duration The duration of the mouse gesture in milliseconds.
	Q_INVOKABLE void drawLink(const QString &fromElementId, const QString &toElementId, int duration);

	/// Returns a list of child nodes on the current diagram with the given metatype.
	/// @param diagram Diagram metatype of the node.
	/// @param element Element metatype of the node.
	/// @returns A string list of graphical identifiers of matching nodes.
	Q_INVOKABLE QStringList nodeList(const QString &diagram, const QString &element);

	/// Creates block with id, like paletteElement in (\a xSceneCoord, \a ySceneCoord) point on scene.
	QString createBlockOnScene(const DraggableElement *paletteElement, int xSceneCoord, int ySceneCoord);

private:
	ScriptAPI &mScriptAPI;
	MainWindow &mMainWindow;
};

}
}
