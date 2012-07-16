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
	explicit MiniMap(QWidget *parent = 0);

	void init(qReal::MainWindow *window);
	void changeSource(int index);

	void setScene(QGraphicsScene *scene);

signals:
	void editorViewRectChanged(QRectF);

public slots:
	void adjustToItems();
	void redrawEditorViewRect();
	void ensureVisible(QList<QRectF> region);

private:
	void setCurrentScene();

	qReal::MainWindow *mWindow;

	QRectF editorViewRect;
};
