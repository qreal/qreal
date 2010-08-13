#pragma once
#include "xmlparser.h"
#include "paintmanager.h"
#include "gesturesmanager.h"

#include <QMainWindow>

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

private:
	Ui::MainWindow *ui;
	PaintManager * mPaintManager;
	QList<QPoint> mPath;
	void paintEvent(QPaintEvent *);
	GesturesManager * mGesturesManager;

protected:
	void mouseMoveEvent(QMouseEvent * event);
	void mousePressEvent(QMouseEvent * event);
	void mouseReleaseEvent(QMouseEvent * event);

private slots:
	void loadFile();
	void drawIdealGesture();
};

