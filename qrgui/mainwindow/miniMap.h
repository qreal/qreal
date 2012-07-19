#pragma once

#include <QtGui>
#include <QObject>

#include "../view/editorView.h"
#include "../view/editorViewScene.h"
#include "mainWindow.h"

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
	enum Mode { None, Drag };

	explicit MiniMap(QWidget *parent = 0);

	void init(qReal::MainWindow *window);
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

	void drawForeground(QPainter *painter, QRectF const &rect);
	/// painting out the areas which aren't to be painted on the minimap (not in the scene rect)
	void drawNonExistentAreas(QPainter *painter, QRectF const &rect);
	/// @return list of areas visible on the minimap but not included in the scene rectangle
	QList<QRectF> getNonExistentAreas(QRectF const &rect);

private:
	void setCurrentScene();
	void clear();
	/// @return a rectangle of the scene which is viewed in the editor view
	QRectF getNewRect();

	qReal::MainWindow *mWindow;

	EditorView *mEditorView;
	/// in the scene coordinates
	QRectF mEditorViewRect;

	Mode mMode;
};
