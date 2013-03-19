#include "dotMatrixDisplay.h"
#include <QPainter>

const int displayHeight = 64;
const int displayWidth = 100;

DotMatrixDisplay::DotMatrixDisplay(QWidget *parent) :
	QWidget(parent)
{
	mMatrix.resize(displayHeight);
	for(int i = 0; i < displayHeight; i++){
		mMatrix[i].resize(displayWidth);
	}

	drawRect(25,25,10,10);
	drawLine(25,25,35,35);
	drawLine(25,35,35,25);
}

void DotMatrixDisplay::paintEvent(QPaintEvent *event){
	QWidget::paintEvent(event);
	QPainter paint(this);

	paint.setBrush(QBrush(Qt::black, Qt::SolidPattern));

	for(int i = 0; i < displayHeight; i++){
		for(int j = 0; j < displayWidth; j++){
			if(mMatrix[i][j]){
				paint.drawRect(i * this->width() / displayWidth
							   ,j * this->height() / displayHeight
							   ,this->width() / displayWidth
							   ,this->height() / displayHeight);
			}
		}
	}
}

void DotMatrixDisplay::clearMatrix(){
	for(int i = 0; i < displayHeight; i++){
		for(int j = 0; j < displayWidth; j++){
			mMatrix[i][j] = false;
		}
	}
	repaint();
}

void DotMatrixDisplay::drawPixel(int x, int y){
	mMatrix[x][y] = true;
	repaint();
}

int sign (int x) {
		return (x > 0) ? 1 : (x < 0) ? -1 : 0;
}

void DotMatrixDisplay::drawLine(int xStart, int yStart, int xEnd, int yEnd){

	int x, y, dx, dy, incrX, incrY, pdx, pdy, es, el, err;
	dx = xEnd - xStart;
	dy = yEnd - yStart;
	incrX = sign(dx);

	incrY = sign(dy);

	if (dx < 0) dx = -dx;
	if (dy < 0) dy = -dy;

	if (dx > dy){
		pdx = incrX;
		pdy = 0;
		es = dy;
		el = dx;
	} else {
		pdx = 0;
		pdy = incrY;
		es = dx;
		el = dy;
	}

	x = xStart;
	y = yStart;
	err = el/2;

	drawPixel (x, y);

	for (int t = 0; t < el; t++){
		err -= es;
		if (err < 0)
		{
			err += el;
			x += incrX;
			y += incrY;
		}
		else
		{
			x += pdx;
			y += pdy;
		}
		drawPixel (x, y);
	}
	repaint();
}


void DotMatrixDisplay::drawRect(int xStart, int yStart, int width, int height){
	drawLine(xStart, yStart, xStart + width, yStart);
	drawLine(xStart, yStart, xStart, yStart + height);
	drawLine(xStart, yStart + height, xStart + width, yStart + height);
	drawLine(xStart + width, yStart, xStart + width, yStart + height);
}



