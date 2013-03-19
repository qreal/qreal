#pragma once

#include <QWidget>
#include <QBitArray>
#include <QPainter>

class DotMatrixDisplay : public QWidget
{
	Q_OBJECT

public:
	explicit DotMatrixDisplay(QWidget *parent = 0);
	void drawPixel(int x, int y);
	void drawLine(int xstart, int ystart, int xend, int yend);
	void clearMatrix();
	void drawRect(int xStart, int yStart, int width, int height);

protected:
	void paintEvent(QPaintEvent *);

private:
	QVector<QBitArray> mMatrix;
	
};
