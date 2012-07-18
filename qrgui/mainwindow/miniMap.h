#pragma once

#include <QtGui>
#include <QObject>

#include "../view/editorView.h"
#include "../view/editorViewScene.h"
#include "mainWindow.h"

class MiniMap : public QGraphicsView
{
	Q_OBJECT
public:
	enum Mode { None, Drag };

	explicit MiniMap(QWidget *parent = 0);

	void init(qReal::MainWindow *window);
	void changeSource(int index);

	void setScene(QGraphicsScene *scene);

signals:
	void editorViewRectChanged(QRectF);

public slots:
	void adjustToItems();
	void ensureVisible(QList<QRectF> region);

protected:
	void wheelEvent(QWheelEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);

	void drawForeground(QPainter *painter, QRectF const &rect);

private:
	void setCurrentScene();
	void clear();
	QRectF getNewRect();

	bool isInEditorRect(QPoint const &point);

	qReal::MainWindow *mWindow;

	EditorView *mEditorView;
	QRectF mEditorViewRect;

	Mode mMode;
};
