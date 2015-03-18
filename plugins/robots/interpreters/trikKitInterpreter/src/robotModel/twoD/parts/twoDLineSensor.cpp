#include "twoDLineSensor.h"

#include <QtCore/QDebug>
#include <QtGui/QImage>

using namespace trik::robotModel::twoD::parts;
using namespace kitBase::robotModel;



LineSensor::LineSensor(const DeviceInfo &info, const PortInfo &port
		, twoDModel::engine::TwoDModelEngineInterface &engine)
	: robotModel::parts::TrikLineSensor(info, port)
	, mEngine(engine)
{
}

void LineSensor::init()
{
}

void LineSensor::detectLine()
{
	QImage image = mEngine.areaUnderSensor(port(), 0.2);
//	image.save("/home/ashatta/img.png");

	int size = image.width() * image.height();
	int red = 0;
	int green = 0;
	int blue = 0;
	for (int x = 0; x < image.width(); ++x) {
		for (int y = 0; y < image.height(); ++y) {
			QRgb pixelColor(image.pixel(x, y));
			red += qRed(pixelColor);
			green += qGreen(pixelColor);
			blue += qBlue(pixelColor);
		}
	}

	mLineColor.setRed(red / size);
	mLineColor.setGreen(green / size);
	mLineColor.setBlue(blue / size);
	qDebug() << mLineColor.red() << mLineColor.green() << mLineColor.blue();
}

void LineSensor::read()
{
	QImage image = mEngine.areaUnderSensor(port(), 2.0);
//	image.save("/home/ashatta/img.png");

	int height = image.height();
	int width = image.width();

	int blacks = 0;
	int horizontalBlacks = 0;
	int horizontalLineWidth = image.height() * 0.2;
	int xCoordinates = 0;
	QVector<QVector<bool> > bitMap(height, QVector<bool>(width, false));
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			bitMap[i][j] = closeEnough(image.pixel(j, i));
			if (bitMap[i][j]) {
				++blacks;
				xCoordinates += i;
				if (((height - horizontalLineWidth) / 2 < i) && (i < (height + horizontalLineWidth) / 2)) {
					++horizontalBlacks;
				}
			}
		}
	}

	const int x = blacks ? qRound((xCoordinates / blacks - width / 2) * 100.0 / (width / 2)) : 0;
	const int lineWidth = blacks / (height * width - blacks);
	const int cross = horizontalBlacks / (height * horizontalLineWidth);
	emit newData({ x, lineWidth, cross });
}

bool LineSensor::closeEnough(const QColor &color) const
{
//	qDebug() << color.red() << color.green() << color.blue();
//	qDebug() << mLineColor.red() << mLineColor.green() << mLineColor.blue();
//	qDebug() << "\n";
//	return qMax(abs(color.red() - mLineColor.red()), qMax(abs(color.green() - mLineColor.green())
//		, abs(color.blue() - mLineColor.blue()))) < 10;
	return color.red() < 10 && color.green() < 10 && color.blue() < 10;
}
