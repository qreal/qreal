#pragma once

#include <QtCore/QObject>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QGraphicsView>


namespace qReal {

class EditorViewScene;
class EditorView;

/** @brief Minimap for the current viewed diagram
*
* Displays the scene of current diagram. The area of the scene, visible in the
* EditorView, displayed on the minimap as a rectangle. Navigation on the scene is possible
* by moving that rectangle with mouse.
*/

class MiniMap : public QGraphicsView
{
	Q_OBJECT
public:
	/// Drag mode is used for dragging a rectangle of the main view
	enum class Mode { None, Drag };

	explicit MiniMap(QWidget *tab);

	void changeSource(int index);

	void setScene(QGraphicsScene *scene);

public slots:
	/// makes the minimap display only items of the scene
	void showScene();
	void ensureVisible(QList<QRectF> region);

protected:
	void wheelEvent(QWheelEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void resizeEvent(QResizeEvent *event);

private:
	void setCurrentScene();
	void clear();
	/// @return a rectangle of the scene which is viewed in the editor view
	QRectF getNewRect();

	EditorView *mEditorView;
	/// in the scene coordinates
	QRectF mEditorViewRect;

	Mode mMode;
};

}
